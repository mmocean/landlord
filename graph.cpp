/*************************************************************************
	> File Name: graph.cpp
	> Author: lihy
	> Mail: lihaiyangcsu@gmail.com 
	> Created Time: 2015-09-24
 ************************************************************************/

#include<list>
#include<cstring>
#include<iostream>
#include <sys/ioctl.h>
using namespace std;


typedef short int s_int;
typedef unsigned char u_char;

u_char canvas[20][122] = {0};

static char poker[] = { '$', '@', 'A', 'K', 'Q', 'J', '10', '9', '8', '7', '6', '5', '4', '3', '2', '1' };
static char color[] = { '+', '-', '*', '/' };
static int serial = 0;


#define SWAP( X, Y )\
do{\
	typeof(X) _x = (X);\
	typeof(Y) _y= (Y);\
	void( &_x == &_y );\
	typeof(X) tmp = _x;\
	(X) = _y;\
	(Y) = tmp;\
}while(0)


/*
 *       (y) ^            point
 *           |           /
 *           |----------P (a,b)
 *         b |          |
 *           |       a  |
 *           '----------'------------> (x)
 *        (0,0)   (coordinate system)
 * */


class CPoint
{
public:
	//CPoint() {}
	
	CPoint( s_int x, s_int y ) : m_x( x ), m_y( y ) {}

	CPoint( s_int x, s_int y, u_char pixel ) : m_x( x ), m_y( y ), m_pixel( pixel ) {}

	CPoint( const CPoint& point )
	{
		m_x = point.m_x;
		m_y = point.m_y;
		m_pixel = point.m_pixel;
	}

	CPoint &operator=( const CPoint& point )
	{
		if( this != &point )
		{
			m_x = point.m_x;
			m_y = point.m_y;
			m_pixel = point.m_pixel;
		}
		return *this;
	}

	bool operator==( const CPoint& point )
	{
		return ( (m_x == point.m_x ) && (m_y == point.m_y) );
	}
	
	~CPoint() {}

public:
	inline int Draw( void )
	{
		//global variable 
		cout<<"m_x: "<<m_x<<" m_y: "<<m_y<<" m_pixel: "<<m_pixel<<endl;
		canvas[20-m_y][m_x] = m_pixel;	
		return 0;
	}

public:
	inline void SetX( s_int x )
	{
		m_x = x;
	}

	inline void SetY( s_int y )
	{
		m_y = y;
	}

	inline void SetPixel( u_char pixel = 0x20 )
	{
		m_pixel = pixel;
	}

	inline s_int GetX( void ) const
	{
		return m_x;
	}

	inline s_int GetY( void ) const
	{
		return m_y;
	}

	inline u_char GetPixel( void ) const
	{
		return m_pixel;
	}

private:
	s_int m_x;
	s_int m_y;
	u_char m_pixel;
};


class CLine
{
public:
	//CLine() {}

	CLine( CPoint x, CPoint y ) : m_pointx( x ), m_pointy( y ) {}
	
	CLine( CPoint x, CPoint y, u_char pixel ) : m_pointx( x ), m_pointy( y ), m_pixel( pixel ) {}
	
	~CLine() {}

public:
	inline int Draw( void )
	{	
		cout<<"CLine Draw"<<endl;
		if( !(m_pointx == m_pointy) )	
		{
			s_int xx = m_pointx.GetX();	
			s_int xy = m_pointx.GetY();	
			s_int yx = m_pointy.GetX();	
			s_int yy = m_pointy.GetY();	
		
			//straight line only
			if( (xx == yx) || (xy == yy) )
			{
				if( xx == yx )
				{
					if( yy > xy )
					{
						SWAP( yy, xy );
					}
					for( int i = yy; i <= xy; ++i )
					{
						CPoint point( xx, i, m_pixel );
						point.Draw();
					}
				} else {
					if( xx > yx )
					{
						SWAP( xx, yx );
					}
					for( int i = xx; i <= yx; ++i )
					{
						CPoint point( i, yy, m_pixel );
						point.Draw();
					}		
				}
				if( m_pointx.GetPixel() != m_pixel )
					m_pointx.Draw();
				if( m_pointy.GetPixel() != m_pixel )
					m_pointy.Draw();
			}
		}
		return 0;
	}	


public:
	inline void SetPointX( const CPoint& point )
	{
		m_pointx = point;	
	}

	inline void SetPointY( const CPoint& point )
	{
		m_pointy = point;	
	}

	inline void SetPixel( u_char pixel )
	{
		m_pixel = pixel;
	}

	inline u_char GetPixel( void ) const
	{
		return m_pixel;
	}

	inline CPoint GetPointX( void ) const
	{
		return m_pointx;
	}

	inline CPoint GetPointY( void ) const
	{
		return m_pointy;
	}

private:
	u_char m_pixel;
	class CPoint m_pointx;
	class CPoint m_pointy;
};


class CRectangle
{
	enum VERTEX { TR, BL, BR };
	enum EDGE { TOP, BOTTOM, LEFT, RIGHT };
public:
	CRectangle( CPoint point, s_int width, s_int height ): m_width( width ), m_height( height ), m_point( point ) 
	{
		m_vertex[TR] = m_vertex[BL] = m_vertex[BR] = '.';
		m_edge[TOP] = m_edge[BOTTOM] = m_edge[LEFT] = m_edge[RIGHT] = '.';
	}
	
	virtual ~CRectangle() {}

public:
	inline int Draw( void )
	{	
		//top-left
		// is m_point

		//top-right
		CPoint tr( m_point.GetX()+m_width, m_point.GetY(), m_vertex[TR] );
		//bottom-left
		CPoint bl( m_point.GetX(), m_point.GetY()-m_height, m_vertex[BL] );
		//bottom-right
		CPoint br( m_point.GetX()+m_width, m_point.GetY()-m_height, m_vertex[BR] );
		
		CLine top( m_point, tr, m_edge[TOP] );
		CLine bottom( bl, br, m_edge[BOTTOM] );
		CLine left( m_point, bl, m_edge[LEFT] );
		CLine right( tr, br, m_edge[RIGHT] );
		
		top.Draw();
		bottom.Draw();
		left.Draw();
		right.Draw();

		return 0;
	}

	inline int Padding( u_char filler = 0x20 )
	{
		s_int x = m_point.GetX();
		s_int y = m_point.GetY();
		for( int i = 0; i <= m_width; ++i )
		{
			for( int j = 0; j <= m_height; ++j )
			{
				CPoint point( x+i, y-j, filler );
				point.Draw();
			}	
		}
		return 0;
	}

	
	inline int SetEdge( u_char top, u_char bottom, u_char left, u_char right )
	{
		m_edge[TOP] = top;
		m_edge[BOTTOM] = bottom;
		m_edge[LEFT] = left;
		m_edge[RIGHT] = right;
	
		return 0;
	}

	inline int SetVertex( u_char tr, u_char bl, u_char br )
	{
		m_vertex[TR] = tr;
		m_vertex[BL] = bl;
		m_vertex[BR] = br;

		return 0;
	}

private:
	s_int m_width;
	s_int m_height;
	u_char m_edge[4];
	u_char m_vertex[3];
	class CPoint m_point;
};


class CCanvas 
{
public:
	CCanvas() 
	{
		struct winsize size;
		ioctl(STDIN_FILENO,TIOCGWINSZ,&size);
		m_row = size.ws_row;
		m_column = size.ws_col;
	}

	CCanvas( s_int row, s_int column ): m_row( row ), m_column( column ) {}
	
	CCanvas( const CCanvas& canvas );

	CCanvas& operator=( const CCanvas& canvas );

public:
	inline void Paint( void )
	{
		cout<<*canvas<<endl;
	}

	static inline void Clear( void )
	{
		cout<<"\033c"<<endl;
	}

private:
	s_int m_row;
	s_int m_column;
	u_char *m_canvas;
};


class CRender
{

};


class CLayout
{
};


class CCard
{
enum COLOR { SPADE, HEART, DIAMOND, CLUB };

public:
	~CCard() {}

private:
	COLOR m_color;
};

class CPlayer
{


private:
	int role;
	std::list<CCard> m_card;	
};


class CReferee
{

};



int main()
{
	CPoint p1(13,14,'.');	
	CPoint p2(13,104,'.');	
	CPoint p3(3,14,'.');	
	CLine l1( p1, p2, '-' );
	CLine l2( p1, p3, '!' );
	memset( canvas, ' ', sizeof(canvas) );
//	l1.Draw();
//	l2.Draw();

	for( int i = 1; i<10; ++i )
	{
		CPoint p4( 30+5*i, 6, '.');	
		CRectangle r( p4, 5, 5 );
		r.SetVertex( '.', '`', '\'' );
		r.SetEdge( '-', '-', '!', '!' );
		r.Draw();
	}

	for( int i = 0; i<5; ++i )
	{
		CPoint p4( 10,20-2*i,'.');	
		CRectangle r( p4, 6, 4 );
		r.SetVertex( '.', '`', '\'' );
		r.SetEdge( '-', '-', '|', '|' );
		r.Padding( ' ' );
		r.Draw();
	}

	for( int i = 0; i<5; ++i )
	{
		CPoint p4( 100,20-2*i,'.');	
		CRectangle r( p4, 6, 4 );
		r.SetVertex( '.', '`', '\'' );
		r.SetEdge( '-', '-', '|', '|' );
		r.Padding( ' ' );
		r.Draw();
	}

	CRectangle r( CPoint( 60, 16, '.'), 5, 5 );
	r.Draw();
	CRectangle r1( CPoint( 80, 10, '.'), 8, 5 );
	r1.Padding( '0' );
	r1.Draw();

	int count = 1;
	CCanvas cc;
	while( count-- > 0 )
	{
		CCanvas::Clear();
		CRectangle r2( CPoint( 65, 16, '.'), 5, 5 );
		r2.SetVertex( '.', '`', '\'' );
		r2.SetEdge( '-', '-', '!', '!' );
		r2.Draw();
		cc.Paint();
		sleep(1);
	}

	return 0;
}






