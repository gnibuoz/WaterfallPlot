
#pragma once
#include <QwtPlot>

#include "qwt_matrix_raster_data.h"
#include "qwt_interval.h"

class SpectrogramData : public QwtMatrixRasterData
{
public:
	SpectrogramData()
	{
		setInterval(Qt::XAxis, QwtInterval(88e6, 108e6));      // 频率 88-108 MHz
		setInterval(Qt::YAxis, QwtInterval(0, 10));			   // 时间
		setInterval(Qt::ZAxis, QwtInterval(0, 1));			   // 场强 0 - 1
	}

	/**
	 * 随机生成一个时刻的数据
	 */
	void addRandomData()
	{
		const auto frequencyCount = 300;				// 频点个数

		const float HalfCol = frequencyCount * 0.5;		//

		auto t = rawData.size() / frequencyCount;

		for (auto f = 0; f < frequencyCount; ++f)
		{
			auto v = sin((t + 100.0) / (1.0*t)*1.57) * cos((f - HalfCol) / HalfCol * 3.14) + 0.2 * sin(qrand() % 100 / 100.0);
			rawData.push_back(v);
		}

		setValueMatrix(rawData, frequencyCount);
		setInterval(Qt::YAxis, QwtInterval(0, t));      // 时间
	}

	QVector<double> rawData;
};


class QwtPlotSpectrogram;

class Plot : public QwtPlot
{
	Q_OBJECT
  public:
	  Plot(QWidget* = NULL);

	  inline SpectrogramData* getData() { return _data; }

private:
	SpectrogramData* _data;
	
	QwtPlotSpectrogram* _spectrogram;
};
