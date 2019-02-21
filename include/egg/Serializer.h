#ifndef NIUBI_SG_SERIALIZER_H
#define NIUBI_SG_SERIALIZER_H

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

#if _WIN32
typedef unsigned int uint;
#endif

#define NBSG_HEX_SIZE 2 // 2="FF" 4="0xFF"

namespace egg
{

    /*
    * 序列器
    */

    class Serializer
    {
    public:
        Serializer(){}
        virtual ~Serializer(){}


        //只支持四种类型：
        //      普通数值    int float
        //      特殊数值    bool enum  直接当作 uint处理
        //      字符串数值
        //      二进制数值

        virtual void readNumber(  std::istream& is ,  char& n ) = 0;
        virtual void writeNumber( std::ostream& os , const char& n ) = 0;
        virtual void readNumber(  std::istream& is ,  unsigned char& n ) = 0;
        virtual void writeNumber( std::ostream& os , const unsigned char& n ) = 0;

        virtual void readNumber(  std::istream& is ,  short& n ) = 0;
        virtual void writeNumber( std::ostream& os , const short& n ) = 0;
        virtual void readNumber(  std::istream& is ,  unsigned short& n ) = 0;
        virtual void writeNumber( std::ostream& os , const unsigned short& n ) = 0;

        virtual void readNumber(  std::istream& is ,  int& n ) = 0;
        virtual void writeNumber( std::ostream& os , const int& n ) = 0;
        virtual void readNumber(  std::istream& is ,  unsigned int& n ) = 0;
        virtual void writeNumber( std::ostream& os , const unsigned int& n ) = 0;

        virtual void readNumber(  std::istream& is ,  long& n ) = 0;
        virtual void writeNumber( std::ostream& os , const long& n ) = 0;
        virtual void readNumber(  std::istream& is ,  unsigned long& n ) = 0;
        virtual void writeNumber( std::ostream& os , const unsigned long& n ) = 0;

        virtual void readNumber(  std::istream& is ,  long long& n ) = 0;
        virtual void writeNumber( std::ostream& os , const long long& n ) = 0;
        virtual void readNumber(  std::istream& is ,  unsigned long long& n ) = 0;
        virtual void writeNumber( std::ostream& os , const unsigned long long& n ) = 0;

        virtual void readNumber(  std::istream& is ,  float& n ) = 0;
        virtual void writeNumber( std::ostream& os , const float& n ) = 0;

        virtual void readNumber(  std::istream& is ,  double& n ) = 0;
        virtual void writeNumber( std::ostream& os , const double& n ) = 0;

        virtual void readString(  std::istream& is , std::string& s ) = 0;
        virtual void writeString( std::ostream& os , const std::string& s ) = 0;

        virtual void readBinary(  std::istream& is , std::vector<unsigned char>& b ) = 0;
        virtual void writeBinary( std::ostream& os , const std::vector<unsigned char>& b ) = 0;

    };

    class Serializer_Text
            :public Serializer
    {
    public:
        Serializer_Text(){}
        virtual ~Serializer_Text(){}

        virtual void readNumber(  std::istream& is , char& n )                  { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const char& n )            { os << n << std::endl; }
        virtual void readNumber(  std::istream& is , unsigned char& n )         { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const unsigned char& n )   { os << n << std::endl; }

        virtual void readNumber(  std::istream& is , short& n )                 { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const short& n )           { os << n << std::endl; }
        virtual void readNumber(  std::istream& is , unsigned short& n )        { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const unsigned short& n )  { os << n << std::endl; }

        virtual void readNumber(  std::istream& is , int& n )                   { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const int& n )             { os << n << std::endl; }
        virtual void readNumber(  std::istream& is , unsigned int& n )          { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const unsigned int& n )    { os << n << std::endl; }

        virtual void readNumber(  std::istream& is , long& n )                  { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const long& n )            { os << n << std::endl; }
        virtual void readNumber(  std::istream& is , unsigned long& n )         { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const unsigned long& n )   { os << n << std::endl; }

        virtual void readNumber(  std::istream& is , long long& n )             { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const long long& n )       { os << n << std::endl; }
        virtual void readNumber(  std::istream& is , unsigned long long& n )    { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const unsigned long long& n )  { os << n << std::endl; }

        virtual void readNumber(  std::istream& is , float& n )                 { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const float& n )           { os << n << std::endl; }

        virtual void readNumber(  std::istream& is , double& n )                { is >>std::dec>> n; }
        virtual void writeNumber( std::ostream& os , const double& n )          { os << n << std::endl; }


        virtual void readString( std::istream& is , std::string& s )
        { 
            unsigned int size = 0;
            is >>std::dec>> size;
            if(size==0) return;
            is.seekg(1, is.cur);

            s.resize(size);
            is.read( &s[0] , size );
        }
        virtual void writeString( std::ostream& os , const std::string& s )
        { 
            os << (unsigned int)(s.size()) << " ";//std::endl;
            os << s.c_str() << std::endl;
        }


        virtual void readBinary(  std::istream& is , std::vector<unsigned char>& b )
        {
            unsigned int size = 0;
            is >>std::dec>> size >> std::hex;
            if(size==0) return;
            is.seekg(1, is.cur);

            b.resize(size/2);
            char buffer[NBSG_HEX_SIZE+1]; buffer[NBSG_HEX_SIZE]='\0';
            for (unsigned int i = 0; i < size; i+=NBSG_HEX_SIZE)
            {
                is.read( buffer , NBSG_HEX_SIZE );
                is.seekg(1, is.cur);
                b[i/NBSG_HEX_SIZE] = (unsigned char)strtol(buffer,0,16);
            }
        }
        virtual void writeBinary( std::ostream& os , const std::vector<unsigned char>& b )
        {
            os << (unsigned int)(b.size()*2) ;//<< " ";//std::endl;
            os << std::hex;
            for (unsigned int i = 0; i < b.size(); i++)
            {
                os << " ";
#if( NBSG_HEX_SIZE == 4 )
                os << "0x";
#endif
                os << std::setw(2) << std::setfill ('0') << (0xff & (b[i]));
            }
            os << std::dec <<std::endl;
        }


    };


    class Serializer_Binary
            :public Serializer
    {
    public:
        Serializer_Binary(){}
        virtual ~Serializer_Binary(){}

        virtual void readNumber(  std::istream& is , char& n )                  { is.read ((char*)&n,sizeof(char)); }
        virtual void writeNumber( std::ostream& os , const char& n )            { os.write((char*)&n,sizeof(char)); }
        virtual void readNumber(  std::istream& is , unsigned char& n )         { is.read ((char*)&n,sizeof(unsigned char)); }
        virtual void writeNumber( std::ostream& os , const unsigned char& n )   { os.write((char*)&n,sizeof(unsigned char)); }

        virtual void readNumber(  std::istream& is , short& n )                 { is.read ((char*)&n,sizeof(short)); }
        virtual void writeNumber( std::ostream& os , const short& n )           { os.write((char*)&n,sizeof(short)); }
        virtual void readNumber(  std::istream& is , unsigned short& n )        { is.read ((char*)&n,sizeof(unsigned short)); }
        virtual void writeNumber( std::ostream& os , const unsigned short& n )  { os.write((char*)&n,sizeof(unsigned short)); }

        virtual void readNumber(  std::istream& is , int& n )                   { is.read ((char*)&n,sizeof(int)); }
        virtual void writeNumber( std::ostream& os , const int& n )             { os.write((char*)&n,sizeof(int)); }
        virtual void readNumber(  std::istream& is , unsigned int& n )          { is.read ((char*)&n,sizeof(unsigned int)); }
        virtual void writeNumber( std::ostream& os , const unsigned int& n )    { os.write((char*)&n,sizeof(unsigned int)); }

        virtual void readNumber(  std::istream& is , long& n )                  { is.read ((char*)&n,sizeof(long)); }
        virtual void writeNumber( std::ostream& os , const long& n )            { os.write((char*)&n,sizeof(long)); }
        virtual void readNumber(  std::istream& is , unsigned long& n )         { is.read ((char*)&n,sizeof(unsigned long)); }
        virtual void writeNumber( std::ostream& os , const unsigned long& n )   { os.write((char*)&n,sizeof(unsigned long)); }

        virtual void readNumber(  std::istream& is , long long& n )             { is.read ((char*)&n,sizeof(long long)); }
        virtual void writeNumber( std::ostream& os , const long long& n )       { os.write((char*)&n,sizeof(long long)); }
        virtual void readNumber(  std::istream& is , unsigned long long& n )    { is.read ((char*)&n,sizeof(unsigned long long)); }
        virtual void writeNumber( std::ostream& os , const unsigned long long& n )  { os.write((char*)&n,sizeof(unsigned long long)); }

        virtual void readNumber(  std::istream& is , float& n )                 { is.read ((char*)&n,sizeof(float)); }
        virtual void writeNumber( std::ostream& os , const float& n )           { os.write((char*)&n,sizeof(float)); }

        virtual void readNumber(  std::istream& is , double& n )                { is.read ((char*)&n,sizeof(double)); }
        virtual void writeNumber( std::ostream& os , const double& n )          { os.write((char*)&n,sizeof(double)); }


        virtual void readString( std::istream& is , std::string& s )
        { 
            unsigned int size = 0;
            is.read ((char*)&size,sizeof(unsigned int));
            if(size==0) return;

            s.resize(size);
            is.read( &s[0] , size );
        }
        virtual void writeString( std::ostream& os , const std::string& s )
        {
            unsigned int size = s.size();
            os.write((char*)&size,sizeof(unsigned int));

            os.write( &s[0] , size );
        }


        virtual void readBinary(  std::istream& is , std::vector<unsigned char>& b )
        {
            unsigned int size = 0;
            is.read ((char*)&size,sizeof(unsigned int));
            if(size==0) return;

            b.resize(size);
            is.read( (char*)&b[0] , size );
        }
        virtual void writeBinary( std::ostream& os , const std::vector<unsigned char>& b )
        {
            unsigned int size = b.size();
            os.write((char*)&size,sizeof(unsigned int));
            if(size==0) return;

            os.write( (char*)&b[0] , size );
        }

    };




}//namespace nbSG

#endif//NIUBI_SG_SERIALIZER_H
