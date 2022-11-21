/*****************************************************************************
 * Qwt Examples - Copyright (C) 2002 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#include "Plot.h"

#include <QwtColorMap>
#include <QwtPlotSpectrogram>
#include <QwtScaleWidget>
#include <QwtScaleDraw>
#include <QwtPlotZoomer>
#include <QwtPlotPanner>
#include <QwtPlotLayout>
#include <QwtPlotRenderer>
#include <QwtInterval>
#include <QwtPainter>
#include <QwtMatrixRasterData>
#include <QTimer>

#include <QPrinter>
#include <QPen>
#include <QPrintDialog>
#include <QElapsedTimer>
#include "vgwaterfallplot.h"

namespace
{
	class MyZoomer : public QwtPlotZoomer
	{
	public:
		MyZoomer(QWidget* canvas)
			: QwtPlotZoomer(canvas)
		{
			setTrackerMode(AlwaysOn);
		}

		virtual QwtText trackerTextF(const QPointF& pos) const QWT_OVERRIDE
		{
			QColor bg(Qt::white);
			bg.setAlpha(200);

			QwtText text = QwtPlotZoomer::trackerTextF(pos);
			text.setBackgroundBrush(QBrush(bg));
			return text;
		}
	};

	class LinearColorMap : public QwtLinearColorMap
	{
	public:
		LinearColorMap(int formatType)
			: QwtLinearColorMap(Qt::blue, Qt::red)
		{
			setFormat((QwtColorMap::Format) formatType);

			addColorStop(0.25, Qt::cyan);
			addColorStop(0.5, Qt::green);
			addColorStop(0.75, Qt::yellow);
		}
	};

	class Spectrogram : public QwtPlotSpectrogram
	{
	public:
		int elapsed() const
		{
			return m_elapsed;
		}

		QSize renderedSize() const
		{
			return m_renderedSize;
		}

	protected:
		virtual QImage renderImage(
			const QwtScaleMap& xMap, const QwtScaleMap& yMap,
			const QRectF& area, const QSize& imageSize) const QWT_OVERRIDE
		{
			QElapsedTimer t;
			t.start();

			QImage image = QwtPlotSpectrogram::renderImage(
				xMap, yMap, area, imageSize);

			m_elapsed = t.elapsed();
			m_renderedSize = imageSize;

			return image;
		}

	private:
		mutable int m_elapsed;
		mutable QSize m_renderedSize;
	};
}

Plot::Plot(QWidget* parent)
	: QwtPlot(parent)
{
	_spectrogram = new Spectrogram();
	_spectrogram->setRenderThreadCount(0); // use system specific thread count
	_spectrogram->setCachePolicy(QwtPlotRasterItem::PaintCache);

	_data = new SpectrogramData;

	_spectrogram->setData(_data);
	_spectrogram->attach(this);

	const QwtInterval zInterval = _spectrogram->data()->interval(Qt::ZAxis);

	// A color bar on the right axis
	QwtScaleWidget* rightAxis = axisWidget(QwtAxis::YRight);
	rightAxis->setTitle("场强");
	rightAxis->setColorBarEnabled(true);

	setAxisScale(QwtAxis::YRight, zInterval.maxValue(), zInterval.minValue());

	const QwtInterval yInterval = _spectrogram->data()->interval(Qt::YAxis);

	QwtScaleWidget* leftAxis = axisWidget(QwtAxis::YLeft);
	leftAxis->setTitle("时间(s)");

	QwtScaleWidget* bottomAxis = axisWidget(QwtAxis::XBottom);
	bottomAxis->setTitle("频率(MHz)");

	setAxisVisible(QwtAxis::YRight);

	plotLayout()->setAlignCanvasToScales(true);

	QwtPlotZoomer* zoomer = new MyZoomer(canvas());
	zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
		Qt::RightButton, Qt::ControlModifier);
	zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
		Qt::RightButton);

	QwtPlotPanner* panner = new QwtPlotPanner(canvas());
	panner->setAxisEnabled(QwtAxis::YRight, false);
	panner->setMouseButton(Qt::MiddleButton);

	const int extent = QwtPainter::horizontalAdvance(
		axisWidget(QwtAxis::YLeft)->fontMetrics(), "100.00");

	axisScaleDraw(QwtAxis::YLeft)->setMinimumExtent(extent);

	const QColor c(Qt::darkBlue);
	zoomer->setRubberBandPen(c);
	zoomer->setTrackerPen(c);

	QwtScaleWidget* axis = axisWidget(QwtAxis::YRight);

	const QwtColorMap::Format format = QwtColorMap::RGB;

	_spectrogram->setColorMap(new LinearColorMap(format));
	axis->setColorMap(zInterval, new LinearColorMap(format));
}