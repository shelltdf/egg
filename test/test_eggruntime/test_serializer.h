
#include <egg/Serializer.h>


void test_serializer(egg::Serializer* ser, std::stringstream& oss)
{
    ser->writeNumber(oss, 'a');
    ser->writeNumber(oss, short(11));
    ser->writeNumber(oss, int(22));
    ser->writeNumber(oss, long(33));
    ser->writeNumber(oss, long long(44));
    ser->writeNumber(oss, float(55.5));
    ser->writeNumber(oss, double(66.6));
    ser->writeString(oss, "this is string");

    std::vector<unsigned char> b;
    b.push_back('\x11');
    b.push_back('\x22');
    b.push_back('\x33');
    ser->writeBinary(oss, b);

    char data_char = 0;
    ser->readNumber(oss, data_char);
    EXPECT_EQ(data_char, 'a');

    short data_short = 0;
    ser->readNumber(oss, data_short);
    EXPECT_EQ(data_short, short(11));

    int data_int = 0;
    ser->readNumber(oss, data_int);
    EXPECT_EQ(data_int, int(22));

    long data_long = 0;
    ser->readNumber(oss, data_long);
    EXPECT_EQ(data_long, long(33));

    long long data_long_long = 0;
    ser->readNumber(oss, data_long_long);
    EXPECT_EQ(data_long_long, long long(44));

    float data_float = 0;
    ser->readNumber(oss, data_float);
    EXPECT_EQ(data_float, float(55.5));

    double data_double = 0;
    ser->readNumber(oss, data_double);
    EXPECT_EQ(data_double, double(66.6));

    std::string data_string;
    ser->readString(oss, data_string);
    EXPECT_STREQ(data_string.c_str(), "this is string");

    std::vector<unsigned char> bb;
    ser->readBinary(oss, bb);
    EXPECT_EQ(bb.size(), 3);
    EXPECT_EQ(bb[0], '\x11');
    EXPECT_EQ(bb[1], '\x22');
    EXPECT_EQ(bb[2], '\x33');
}

TEST(Serializer, text)
{
    egg::Serializer_Text* ser = new egg::Serializer_Text();
    std::stringstream oss(std::ios_base::out | std::ios_base::in );

    test_serializer(ser, oss);
}

TEST(Serializer, binary)
{
    egg::Serializer_Binary* ser = new egg::Serializer_Binary();
    std::stringstream oss(std::ios_base::out | std::ios_base::in | std::ios_base::binary);

    test_serializer(ser, oss);
}


//ÒÆÖ²¹ýÀ´µÄ²âÊÔ
void testSerializer()
{

    std::ostringstream oss;
    egg::Serializer* ser = new egg::Serializer_Text();

    //std::ostringstream oss(std::ios::binary);
    //nbSG::Serializer* ser = new nbSG::Serializer_Binary();

    std::string s1 = "111 aasd kjksd \n laksjkj \t aksjkd";
    std::string s2;

    std::vector<unsigned char> b1;
    std::vector<unsigned char> b2;

    b1.push_back('\x99');
    b1.push_back('\xfe');

    ser->writeNumber(oss, int(-999));
    ser->writeNumber(oss, float(-2.365));
    ser->writeNumber(oss, int(1));
    ser->writeString(oss, s1);
    ser->writeBinary(oss, b1);
    ser->writeNumber(oss, int(2222));

    std::string str = oss.str();

    int a = 0;
    float b = 0.0f;
    int c = 0;
    int d = 0;

    std::istringstream is(str);

    ser->readNumber(is, a);
    ser->readNumber(is, b);
    ser->readNumber(is, c);
    ser->readString(is, s2);
    ser->readBinary(is, b2);
    ser->readNumber(is, d);
}


