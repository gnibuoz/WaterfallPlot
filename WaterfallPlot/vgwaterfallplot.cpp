#include "vgwaterfallplot.h"
#include "ui_vgwaterfallplot.h"
#include "QLabel"
#include "QTimer"

VGWaterfallPlot::VGWaterfallPlot(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::VGWaterfallPlot();
	ui->setupUi(this);

	Plot* plot = ui->frame;


	// 生成初始静态数据，实际中需要注释掉....
	int count = 300;
	while (count--)
	{
		plot->getData()->addRandomData();
	}

	// 生成动态演示数据，实际中需要注释掉....
	static QTimer s_timer;
	connect(&s_timer, &QTimer::timeout, [this,plot]() {

		auto _data = plot->getData();

		_data->addRandomData();

		const QwtInterval yInterval = _data->interval(Qt::YAxis);

		const float span = 50.0;

		if (yInterval.width() > span)
		{
			plot->setAxisScale(QwtAxis::YLeft, yInterval.maxValue(), yInterval.maxValue() - span);
		}
		else
			plot->setAxisScale(QwtAxis::YLeft, yInterval.maxValue(), yInterval.minValue());

		plot->replot();
	});

	s_timer.start(200);
}

VGWaterfallPlot::~VGWaterfallPlot()
{
	delete ui;
}