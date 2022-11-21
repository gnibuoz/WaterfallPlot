#pragma once

#include <QWidget>


namespace Ui { class VGWaterfallPlot; };

using FrameData = QVector<qreal>&;

class VGWaterfallPlot : public QWidget
{
	Q_OBJECT

public:
	VGWaterfallPlot(QWidget *parent = Q_NULLPTR);
	~VGWaterfallPlot();

private:
	Ui::VGWaterfallPlot *ui;
};
