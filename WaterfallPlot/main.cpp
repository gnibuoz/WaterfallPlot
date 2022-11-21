#pragma comment(lib,"qwt.lib")

#include <QApplication>
#include "vgwaterfallplot.h"

int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    app.setStyle( "Windows" );

	VGWaterfallPlot plot;
	plot.show();

    return app.exec();
}
