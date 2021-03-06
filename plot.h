#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui.hpp>

namespace nana
{
namespace plot
{
class plot;

/// Single trace to be plotted
class trace
{
public:
    trace()
        : myfRealTime( false )
    {
    }

    ~trace() 
    {
        delete myLabel;    
    }

    /** Convert trace to real time operation
        @param[in] w number of data points to display
        Data points older than w scroll off the left edge of the plot and are lost
    */
    void realTime( int w )
    {
        myfRealTime = true;
        myRealTimeNext = 0;
        myY.clear();
        myY.resize( w );
    }

    /** set static data
        @param[in] y vector of data points to display
        An exception is thrown when this is called
        for a trace that has been converted to real time
    */
    void add( const std::vector< double >& y );

    /** add new value to real time data
        @param[in] y the new data point
        An exception is thrown when this is called
        for a trace that has not been converted to real time
    */
    void add( double y );

    /// set color
    void color( const colors & clr )
    {
        myColor = clr;
        if (myLabel)
          myLabel->fgcolor(clr);
    }

    /** sets the label for trace
        @param[in] myLabelText label text
    */
    void setLabel(const char* myLabelText);

    /// set plot where this trace will appear
    void Plot( plot * p )
    {
        myPlot = p;
    }

    int size()
    {
        return (int) myY.size();
    }

    void bounds( int& min, int& max );

    /// draw
    void update( paint::graphics& graph );

private:
    plot * myPlot;
    std::vector< double > myY;
    colors myColor;
    label * myLabel = nullptr;
    bool myfRealTime;
    int myRealTimeNext;
};

class axis
{
public:
    axis( plot * p );

    ~axis()
    {
        delete myLabelMin;
        delete myLabelMax;
    }

    /// draw
    void update( paint::graphics& graph );

private:
    plot * myPlot;
    label * myLabelMin;
    label * myLabelMax;
};


/** 2D plotting */
class plot
{
public:

    /** CTOR
        @param[in parent window where plot will be drawn
    */
    plot( window parent );

    ~plot()
    {
       delete myAxis;
       for (auto& t: myTrace)
         delete t;
    }

    /** Add static trace
        @return reference to new trace
        The data in a static trace does not change
    */
    trace& AddStaticTrace()
    {
        trace * t = new trace();
        t->Plot( this );
        myTrace.push_back( t );
        return *t;
    }

    /** Add real time trace
        @param[in] w number of recent data points to display
        @return reference to new trace
        The data in a real time trace receives new values from time to time
        The display shows w recent values.  Older values scroll off the
        left hand side of the plot and disappear.
    */
    trace& AddRealTimeTrace( int w )
    {
        trace * t = new trace();
        t->Plot( this );
        t->realTime( w );
        myTrace.push_back( t );
        return *t;
    }

    int xinc()
    {
        return myXinc;
    }
    int minY()
    {
        return myMinY;
    }
    int maxY()
    {
        return myMaxY;
    }
    double Scale()
    {
        return myScale;
    }
    int YOffset()
    {
        return myYOffset;
    }
    window parent()
    {
        return myParent;
    }

    void update()
    {
        API::refresh_window( myParent );
    }

private:

    ///window where plot will be drawn
    window myParent;

    drawing * myDrawing;

    axis * myAxis;

    /// plot traces
    std::vector< trace* > myTrace;

    int myXinc;
    int myMinY, myMaxY;
    double myScale;
    int myYOffset;

    /** calculate scaling factors so plot will fit in window client area
        @param[in] w width
        @param[in] h height
    */
    void CalcScale( int w, int h );

};

}
}
