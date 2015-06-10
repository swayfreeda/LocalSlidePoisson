//
// Created by Luwei Yang on 6/9/15.
//

#ifndef RECONSTRUCTOR_INDEX_H
#define RECONSTRUCTOR_INDEX_H

namespace DDG
{
    class Index
    {
    public:
        Index( void )
        {}

        Index( int p, int t, int n )
                : position( p ), texcoord( t ), normal( n )
        {}

        bool operator<( const Index& i ) const
        {
            if( position < i.position ) return true;
            if( position > i.position ) return false;
            if( texcoord < i.texcoord ) return true;
            if( texcoord > i.texcoord ) return false;
            if(   normal < i.normal   ) return true;
            if(   normal > i.normal   ) return false;
            return false;
        }

        int position;
        int texcoord;
        int normal;
    };
}

#endif //RECONSTRUCTOR_INDEX_H
