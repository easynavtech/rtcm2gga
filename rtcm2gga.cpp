// rtcm2gga.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>

#ifndef WEEK_SECOND_HALF
#define WEEK_SECOND_HALF (7*24*1800)
#endif

#ifndef WEEK_SECOND_FULL 
#define WEEK_SECOND_FULL (WEEK_SECOND_HALF+WEEK_SECOND_HALF)
#endif

#ifndef RTCM3PREAMB
#define RTCM3PREAMB 0xD3        /* rtcm ver.3 frame preamble */
#endif

static const unsigned int tbl_CRC24Q[] = {
    0x000000,0x864CFB,0x8AD50D,0x0C99F6,0x93E6E1,0x15AA1A,0x1933EC,0x9F7F17,
    0xA18139,0x27CDC2,0x2B5434,0xAD18CF,0x3267D8,0xB42B23,0xB8B2D5,0x3EFE2E,
    0xC54E89,0x430272,0x4F9B84,0xC9D77F,0x56A868,0xD0E493,0xDC7D65,0x5A319E,
    0x64CFB0,0xE2834B,0xEE1ABD,0x685646,0xF72951,0x7165AA,0x7DFC5C,0xFBB0A7,
    0x0CD1E9,0x8A9D12,0x8604E4,0x00481F,0x9F3708,0x197BF3,0x15E205,0x93AEFE,
    0xAD50D0,0x2B1C2B,0x2785DD,0xA1C926,0x3EB631,0xB8FACA,0xB4633C,0x322FC7,
    0xC99F60,0x4FD39B,0x434A6D,0xC50696,0x5A7981,0xDC357A,0xD0AC8C,0x56E077,
    0x681E59,0xEE52A2,0xE2CB54,0x6487AF,0xFBF8B8,0x7DB443,0x712DB5,0xF7614E,
    0x19A3D2,0x9FEF29,0x9376DF,0x153A24,0x8A4533,0x0C09C8,0x00903E,0x86DCC5,
    0xB822EB,0x3E6E10,0x32F7E6,0xB4BB1D,0x2BC40A,0xAD88F1,0xA11107,0x275DFC,
    0xDCED5B,0x5AA1A0,0x563856,0xD074AD,0x4F0BBA,0xC94741,0xC5DEB7,0x43924C,
    0x7D6C62,0xFB2099,0xF7B96F,0x71F594,0xEE8A83,0x68C678,0x645F8E,0xE21375,
    0x15723B,0x933EC0,0x9FA736,0x19EBCD,0x8694DA,0x00D821,0x0C41D7,0x8A0D2C,
    0xB4F302,0x32BFF9,0x3E260F,0xB86AF4,0x2715E3,0xA15918,0xADC0EE,0x2B8C15,
    0xD03CB2,0x567049,0x5AE9BF,0xDCA544,0x43DA53,0xC596A8,0xC90F5E,0x4F43A5,
    0x71BD8B,0xF7F170,0xFB6886,0x7D247D,0xE25B6A,0x641791,0x688E67,0xEEC29C,
    0x3347A4,0xB50B5F,0xB992A9,0x3FDE52,0xA0A145,0x26EDBE,0x2A7448,0xAC38B3,
    0x92C69D,0x148A66,0x181390,0x9E5F6B,0x01207C,0x876C87,0x8BF571,0x0DB98A,
    0xF6092D,0x7045D6,0x7CDC20,0xFA90DB,0x65EFCC,0xE3A337,0xEF3AC1,0x69763A,
    0x578814,0xD1C4EF,0xDD5D19,0x5B11E2,0xC46EF5,0x42220E,0x4EBBF8,0xC8F703,
    0x3F964D,0xB9DAB6,0xB54340,0x330FBB,0xAC70AC,0x2A3C57,0x26A5A1,0xA0E95A,
    0x9E1774,0x185B8F,0x14C279,0x928E82,0x0DF195,0x8BBD6E,0x872498,0x016863,
    0xFAD8C4,0x7C943F,0x700DC9,0xF64132,0x693E25,0xEF72DE,0xE3EB28,0x65A7D3,
    0x5B59FD,0xDD1506,0xD18CF0,0x57C00B,0xC8BF1C,0x4EF3E7,0x426A11,0xC426EA,
    0x2AE476,0xACA88D,0xA0317B,0x267D80,0xB90297,0x3F4E6C,0x33D79A,0xB59B61,
    0x8B654F,0x0D29B4,0x01B042,0x87FCB9,0x1883AE,0x9ECF55,0x9256A3,0x141A58,
    0xEFAAFF,0x69E604,0x657FF2,0xE33309,0x7C4C1E,0xFA00E5,0xF69913,0x70D5E8,
    0x4E2BC6,0xC8673D,0xC4FECB,0x42B230,0xDDCD27,0x5B81DC,0x57182A,0xD154D1,
    0x26359F,0xA07964,0xACE092,0x2AAC69,0xB5D37E,0x339F85,0x3F0673,0xB94A88,
    0x87B4A6,0x01F85D,0x0D61AB,0x8B2D50,0x145247,0x921EBC,0x9E874A,0x18CBB1,
    0xE37B16,0x6537ED,0x69AE1B,0xEFE2E0,0x709DF7,0xF6D10C,0xFA48FA,0x7C0401,
    0x42FA2F,0xC4B6D4,0xC82F22,0x4E63D9,0xD11CCE,0x575035,0x5BC9C3,0xDD8538
};

static unsigned int crc24q(const unsigned char* buff, int len)
{
    unsigned int crc = 0;
    int i;

    for (i = 0; i < len; i++) crc = ((crc << 8) & 0xFFFFFF) ^ tbl_CRC24Q[(crc >> 16) ^ buff[i]];
    return crc;
}

extern unsigned int getbitu(const unsigned char* buff, int pos, int len)
{
    unsigned int bits = 0;
    int i;
    for (i = pos; i < pos + len; i++) bits = (bits << 1) + ((buff[i / 8] >> (7 - i % 8)) & 1u);
    return bits;
}
static int getbits(const unsigned char* buff, int pos, int len)
{
    unsigned int bits = getbitu(buff, pos, len);
    if (len <= 0 || 32 <= len || !(bits & (1u << (len - 1)))) return (int)bits;
    return (int)(bits | (~0u << len)); /* extend sign */
}
/* get signed 38bit field ----------------------------------------------------*/
static double getbits_38(const unsigned char* buff, int pos)
{
    return (double)getbits(buff, pos, 32) * 64.0 + getbitu(buff, pos + 32, 6);
}
/* decode type 1005: stationary rtk reference station arp --------------------*/
static int decode_type1005(unsigned char* buff, int len, int* staid, double* pos)
{
    double rr[3];
    int i = 24 + 12, j/*,staid,itrf*/;

    if (i + 140 == len * 8) {
        *staid = getbitu(buff, i, 12); i += 12;
        /*itrf =getbitu(buff,i, 6);*/  i += 6 + 4;
        rr[0] = getbits_38(buff, i);   i += 38 + 2;
        rr[1] = getbits_38(buff, i);   i += 38 + 2;
        rr[2] = getbits_38(buff, i);
    }
    else {
        return -1;
    }

    for (j = 0; j < 3; j++) {
        pos[j] = rr[j] * 0.0001;
    }
    return 5;
}
/* decode type 1006: stationary rtk reference station arp with height --------*/
static int decode_type1006(unsigned char* buff, int len, int* staid, double* pos)
{
    double rr[3]/*,anth*/;
    int i = 24 + 12, j/*,staid,itrf*/;

    if (i + 156 <= len * 8) {
        *staid = getbitu(buff, i, 12); i += 12;
        /*itrf =getbitu(buff,i, 6);*/ i += 6 + 4;
        rr[0] = getbits_38(buff, i); i += 38 + 2;
        rr[1] = getbits_38(buff, i); i += 38 + 2;
        rr[2] = getbits_38(buff, i); i += 38;
        /*anth =getbitu(buff,i,16);*/
    }
    else {
        return -1;
    }

    for (j = 0; j < 3; j++) {
        pos[j] = rr[j] * 0.0001;
    }
    return 5;
}


#ifndef MAX_RTCM_BUF_LEN
#define MAX_RTCM_BUF_LEN 1200
#endif

typedef struct {        /* RTCM control struct type */
    int staid;          /* station id */
    int type;
    int nbyte;          /* number of bytes in message buffer */
    int nbit;           /* number of bits in word buffer */
    int len;            /* message length (bytes) */
    unsigned char buff[MAX_RTCM_BUF_LEN]; /* message buffer */
    double pos[3];
    double tow;
    int wk;
    int sync;
    int crc;
    int slen;
    char sys;
    char prn;
} rtcm_buff_t;


static int add_rtcm_to_buff(rtcm_buff_t* rtcm, unsigned char data)
{
    if (rtcm->sync == 0) rtcm->slen = 0;
    rtcm->type = 0;
    rtcm->crc = 0;
    rtcm->staid = 0;

    if (rtcm->nbyte >= MAX_RTCM_BUF_LEN) rtcm->nbyte = 0;
    if (rtcm->nbyte == 0) {
        if (data != RTCM3PREAMB) return 0;
        rtcm->buff[rtcm->nbyte++] = data;
        return 0;
    }
    rtcm->buff[rtcm->nbyte++] = data;
    return 1;
}

static int input_rtcm3_type(rtcm_buff_t* rtcm, unsigned char data)
{
    int ret = 0, i = 24;
    if (rtcm->sync == 0) rtcm->slen = 0;
    if (add_rtcm_to_buff(rtcm, data) == 0) return 0;

    if (rtcm->nbyte < 3) return 0;
    rtcm->len = getbitu(rtcm->buff, 14, 10) + 3; /* length without parity */
    if (rtcm->nbyte < rtcm->len + 3) return 0;
    rtcm->nbyte = 0;
    i = 24;
    rtcm->type = getbitu(rtcm->buff, i, 12); i += 12;
    /* decode rtcm3 message */
    if ((rtcm->type == 1074 || rtcm->type == 1075 || rtcm->type == 1076 || rtcm->type == 1077) || /* GPS */
        (rtcm->type == 1094 || rtcm->type == 1095 || rtcm->type == 1096 || rtcm->type == 1097) || /* GAL */
        (rtcm->type == 1104 || rtcm->type == 1105 || rtcm->type == 1106 || rtcm->type == 1107) || /* SBS */
        (rtcm->type == 1114 || rtcm->type == 1115 || rtcm->type == 1116 || rtcm->type == 1117))   /* QZS */
    {
        /* GPS, GAL, SBS, QZS */
        rtcm->staid = getbitu(rtcm->buff, i, 12);         i += 12;
        rtcm->tow = getbitu(rtcm->buff, i, 30) * 0.001;   i += 30;
        rtcm->sync = getbitu(rtcm->buff, i, 1);           i += 1;
        ret = rtcm->sync ? 0 : 1;
    }
    if (rtcm->type == 1084 || rtcm->type == 1085 || rtcm->type == 1086 || rtcm->type == 1087)
    {
        /* GLO */
        rtcm->staid = getbitu(rtcm->buff, i, 12);         i += 12;
        double dow = getbitu(rtcm->buff, i, 3);           i += 3;
        double tod = getbitu(rtcm->buff, i, 27) * 0.001;  i += 27;
        rtcm->sync = getbitu(rtcm->buff, i, 1);           i += 1;
        double tow = dow * 24 * 3600 + tod - 3 * 3600 + 18;
        if (rtcm->tow > 0.0 && fabs(tow - rtcm->tow) > (24 * 1800))
        {
            tow -= floor((tow - rtcm->tow) / (24 * 1800)) * (24 * 1800);
        }
        rtcm->tow = tow;
        ret = rtcm->sync ? 0 : 1;
    }
    if (rtcm->type == 1124 || rtcm->type == 1125 || rtcm->type == 1126 || rtcm->type == 1127)
    {
        /* BDS */
        rtcm->staid = getbitu(rtcm->buff, i, 12);          i += 12;
        double tow = getbitu(rtcm->buff, i, 30) * 0.001;   i += 30;
        rtcm->sync = getbitu(rtcm->buff, i, 1);            i += 1;
        tow += 14.0; /* BDT -> GPST */
        rtcm->tow = tow;
        ret = rtcm->sync ? 0 : 1;
    }
    if (rtcm->type == 1019)
    {
        int prn = getbitu(rtcm->buff, i, 6);              i += 6;
        int week = getbitu(rtcm->buff, i, 10);            i += 10;
        rtcm->wk = week + 2048;
        rtcm->sys = 'G';
        rtcm->prn = prn;
    }
    if (rtcm->type == 1020)
    {
        int prn = getbitu(rtcm->buff, i, 6);              i += 6;
        int frq = getbitu(rtcm->buff, i, 5) - 7;          i += 5 + 2 + 2;
        rtcm->sys = 'R';
        rtcm->prn = prn;
    }
    if (rtcm->type == 1042)
    {
        int prn = getbitu(rtcm->buff, i, 6);              i += 6;
        int week = getbitu(rtcm->buff, i, 13);              i += 13;
        rtcm->wk = week + 1356; /* BDT week to GPS week */
    }
    if (rtcm->type == 1044)
    {
        int prn = getbitu(rtcm->buff, i, 4);              i += 4 + 430;
        int week = getbitu(rtcm->buff, i, 10);              i += 10;
        rtcm->wk = week + 2048;
        rtcm->sys = 'J';
        rtcm->prn = prn;
    }
    if (rtcm->type == 1045 || rtcm->type == 1046)
    {
        int prn = getbitu(rtcm->buff, i, 6);              i += 6;
        int week = getbitu(rtcm->buff, i, 12);              i += 12; /* gst-week */
        rtcm->wk = week + 1024; /* gal-week = gst-week + 1024 */
    }
    if (rtcm->type == 1005)
    {
        ret = decode_type1005(rtcm->buff, rtcm->len, &rtcm->staid, rtcm->pos);
    }
    if (rtcm->type == 1006)
    {
        ret = decode_type1006(rtcm->buff, rtcm->len, &rtcm->staid, rtcm->pos);
    }
    /* check parity */
    if (crc24q(rtcm->buff, rtcm->len) != getbitu(rtcm->buff, rtcm->len * 8, 24)) {
        rtcm->crc = 1;
        return 0;
    }
    rtcm->slen += rtcm->len + 3;
    return ret;
}

static void deg2dms(double deg, double* dms)
{
    double sign = deg < 0.0 ? -1.0 : 1.0, a = fabs(deg);
    dms[0] = floor(a); a = (a - dms[0]) * 60.0;
    dms[1] = floor(a); a = (a - dms[1]) * 60.0;
    dms[2] = a; dms[0] *= sign;
}

#ifndef PI
#define	PI 3.14159265358979
#endif

/* output solution in the form of nmea GGA sentence --------------------------*/
static int outnmea_gga(unsigned char* buff, double time, int type, double* blh, int ns, double dop, double age)
{
    double h, ep[6], dms1[3], dms2[3];
    char* p = (char*)buff, * q, sum;

    if (type != 1 && type != 4 && type != 5) {
        p += sprintf(p, "$GPGGA,,,,,,,,,,,,,,");
        for (q = (char*)buff + 1, sum = 0; *q; q++) sum ^= *q;
        p += sprintf(p, "*%02X%c%c", sum, 0x0D, 0x0A);
        return p - (char*)buff;
    }
    time -= 18.0;
    ep[2] = floor(time / (24 * 3600));
    time -= ep[2] * 24 * 3600.0;
    ep[3] = floor(time / 3600);
    time -= ep[3] * 3600;
    ep[4] = floor(time / 60);
    time -= ep[4] * 60;
    ep[5] = time;
    h = 0.0;
    deg2dms(fabs(blh[0]) * 180.0 / PI, dms1);
    deg2dms(fabs(blh[1]) * 180.0 / PI, dms2);
    p += sprintf(p, "$GPGGA,%02.0f%02.0f%05.2f,%02.0f%010.7f,%s,%03.0f%010.7f,%s,%d,%02d,%.1f,%.3f,M,%.3f,M,%.1f,",
        ep[3], ep[4], ep[5], dms1[0], dms1[1] + dms1[2] / 60.0, blh[0] >= 0 ? "N" : "S",
        dms2[0], dms2[1] + dms2[2] / 60.0, blh[1] >= 0 ? "E" : "W", type,
        ns, dop, blh[2] - h, h, age);
    for (q = (char*)buff + 1, sum = 0; *q; q++) sum ^= *q; /* check-sum */
    p += sprintf(p, "*%02X%c%c", sum, 0x0D, 0x0A);
    return p - (char*)buff;
}

#ifndef ae_WGS84
#define ae_WGS84 6378137.0
#endif
#ifndef finv_WGS84
#define finv_WGS84 298.257223563
#endif

static void xyz2blh(const double* xyz, double* blh)
{
    // ecef xyz => blh
    double a = ae_WGS84, finv = finv_WGS84;
    double f = 1.0 / finv, e2 = 2 * f - f * f;
    double x = xyz[0], y = xyz[1], z = xyz[2], lat, lon, ht;
    double R = sqrt(x * x + y * y + z * z);
    double ang = atan(fabs(z / sqrt(x * x + y * y))) * ((z < 0.0) ? -1.0 : 1.0);
    //if (z<0.0) ang = -ang;
    double lat1 = ang;
    double Rw = sqrt(1 - e2 * sin(lat1) * sin(lat1));
    double Rn = 0.0;
    lat = atan(fabs(tan(ang) * (1 + (a * e2 * sin(lat1)) / (z * Rw))));
    if (z < 0.0) lat = -lat;
    while (fabs(lat - lat1) > 1e-12)
    {
        lat1 = lat;
        Rw = sqrt(1 - e2 * sin(lat1) * sin(lat1));
        lat = atan(fabs(tan(ang) * (1 + (a * e2 * sin(lat1)) / (z * Rw))));
        if (z < 0.0) lat = -lat;
    }
    if (lat > PI) lat = lat - 2.0 * PI;
    if (fabs(x) < 1e-12) { if (y >= 0.0) lon = PI / 2.0; else lon = 3.0 * PI / 2.0; }
    else
    {
        lon = atan(fabs(y / x));
        if (x > 0.0) { if (y >= 0.0) lon = lon; else lon = 2.0 * PI - lon; }
        else { if (y >= 0.0) lon = PI - lon; else lon = PI + lon; }
    }
    Rw = sqrt(1 - e2 * sin(lat) * sin(lat));
    Rn = a / Rw;
    ht = R * cos(ang) / cos(lat) - Rn;
    if (lon > PI) lon = lon - 2.0 * PI;
    blh[0] = lat;
    blh[1] = lon;
    blh[2] = ht;
    return;
}

static void print_kml_heder(FILE* fKML)
{
    // write header for KML 
    if (fKML) {
        fprintf(fKML, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(fKML, "<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n");
        fprintf(fKML, "<Document>\n");
        // fprintf(fKML, "<Placemark>\n");    
        // fprintf(fKML, "<name>extruded</name>\n");
        // fprintf(fKML, "<LineString>\n");
        // fprintf(fKML, "<extrude>1</extrude>\n");
        // fprintf(fKML, "<tessellate>1</tessellate>\n");
        // fprintf(fKML, "<altitudeMode>relativeToGround</altitudeMode>\n");
        // fprintf(fKML, "<coordinates>\n"); 
        fprintf(fKML, "<Style id=\"spp\">\n");
        fprintf(fKML, "<IconStyle>\n");
        fprintf(fKML, "<color>ff0000ff</color>\n");
        fprintf(fKML, "<scale>0.300</scale>\n");
        fprintf(fKML, "<Icon>\n");
        fprintf(fKML, "<href>http://maps.google.com/mapfiles/kml/shapes/track.png</href>\n");
        fprintf(fKML, "</Icon>\n");
        fprintf(fKML, "</IconStyle>\n");
        fprintf(fKML, "</Style>\n");
        fprintf(fKML, "<Style id=\"rtd\">\n");
        fprintf(fKML, "<IconStyle>\n");
        fprintf(fKML, "<color>ffff00ff</color>\n");
        fprintf(fKML, "<scale>0.300</scale>\n");
        fprintf(fKML, "<Icon>\n");
        fprintf(fKML, "<href>http://maps.google.com/mapfiles/kml/shapes/track.png</href>\n");
        fprintf(fKML, "</Icon>\n");
        fprintf(fKML, "</IconStyle>\n");
        fprintf(fKML, "</Style>\n");
        fprintf(fKML, "<Style id=\"udr\">\n");
        fprintf(fKML, "<IconStyle>\n");
        fprintf(fKML, "<color>50FF78F0</color>\n");
        fprintf(fKML, "<scale>0.300</scale>\n");
        fprintf(fKML, "<Icon>\n");
        fprintf(fKML, "<href>http://maps.google.com/mapfiles/kml/shapes/track.png</href>\n");
        fprintf(fKML, "</Icon>\n");
        fprintf(fKML, "</IconStyle>\n");
        fprintf(fKML, "</Style>\n");
        fprintf(fKML, "<Style id=\"fix\">\n");
        fprintf(fKML, "<IconStyle>\n");
        fprintf(fKML, "<color>ff00ff00</color>\n");
        fprintf(fKML, "<scale>0.300</scale>\n");
        fprintf(fKML, "<Icon>\n");
        fprintf(fKML, "<href>http://maps.google.com/mapfiles/kml/shapes/track.png</href>\n");
        fprintf(fKML, "</Icon>\n");
        fprintf(fKML, "</IconStyle>\n");
        fprintf(fKML, "</Style>\n");
        fprintf(fKML, "<Style id=\"flt\">\n");
        fprintf(fKML, "<IconStyle>\n");
        fprintf(fKML, "<color>ff00aaff</color>\n");
        fprintf(fKML, "<scale>0.300</scale>\n");
        fprintf(fKML, "<Icon>\n");
        fprintf(fKML, "<href>http://maps.google.com/mapfiles/kml/shapes/track.png</href>\n");
        fprintf(fKML, "</Icon>\n");
        fprintf(fKML, "</IconStyle>\n");
        fprintf(fKML, "</Style>\n");
    }
    return;
}

static void print_kml_gga(FILE* fKML, double lat, double lon, double ht, int solType, double time, float heading, char* sol_status)
{
    int day = 0;
    int hh = 0;
    int mm = 0;
    if (fKML == NULL) return;
    if (lat == 0.0 || lon == 0.0 || ht == 0.0) return;
    if (fKML) {
        day = (int)floor(time / (3600 * 24));
        time -= day * 24 * 3600;
        hh = (int)floor(time / 3600);
        time -= hh * 3600;
        mm = (int)floor(time / 60);
        time -= mm * 60;
        fprintf(fKML, "<Placemark>\n");
        if (solType == 1) {
            fprintf(fKML, "<styleUrl>#spp</styleUrl>\n");
        }
        else if (solType == 4) {
            fprintf(fKML, "<styleUrl>#fix</styleUrl>\n");
        }
        else if (solType == 5) {
            fprintf(fKML, "<styleUrl>#flt</styleUrl>\n");
        }
        else if (solType == 2) {
            fprintf(fKML, "<styleUrl>#rtd</styleUrl>\n");
        }
        else if (solType == 3) {
            fprintf(fKML, "<styleUrl>#udr</styleUrl>\n");
        }
        fprintf(fKML, "<Style>\n");
        fprintf(fKML, "<IconStyle>\n");
        fprintf(fKML, "<heading>%f</heading>\n", heading);
        fprintf(fKML, "</IconStyle>\n");
        fprintf(fKML, "</Style>\n");
        fprintf(fKML, "<ExtendedData>\n");
        fprintf(fKML, "<Data name=\"time\">\n");
        fprintf(fKML, "<value>%02i:%02i:%5.2f</value>\n", hh, mm, time);
        fprintf(fKML, "</Data>\n");
        fprintf(fKML, "<Data name=\"heading\">\n");
        fprintf(fKML, "<value>%.2f</value>\n", heading);
        fprintf(fKML, "</Data>\n");
        fprintf(fKML, "<Data name=\"sol_status\">\n");
        fprintf(fKML, "<value>%s</value>\n", sol_status);
        fprintf(fKML, "</Data>\n");
        fprintf(fKML, "</ExtendedData>\n");
        fprintf(fKML, "<Point>\n");
        fprintf(fKML, "<coordinates>%14.9f,%14.9f,%14.4f</coordinates>\n", lon, lat, ht);
        fprintf(fKML, "</Point>\n");
        fprintf(fKML, "</Placemark>\n");
    }
    return;
}

static void print_kml_eof(FILE* fKML)
{
    if (fKML)
    {
        // fprintf(fKML, "</coordinates>\n");    
        // fprintf(fKML, "</LineString>\n");
        // fprintf(fKML, "</Placemark>\n");
        fprintf(fKML, "</Document>\n");
        fprintf(fKML, "</kml>\n");

    }
}

typedef struct
{
    unsigned int type;
    unsigned int count;
    double time;
}msg_t;

typedef struct
{
    double start_time;
    double end_time;
}gap_t;

static void rtcm2gga(const char* fname)
{
    FILE* fRTCM = fopen(fname, "rb"); if (fRTCM == NULL) return;

    FILE* fLOG = NULL;

    char fileName[255] = { 0 };
    char outfilename[255] = { 0 };

    strcpy(fileName, fname);
    char* result = strrchr(fileName, '.');
    if (result != NULL) result[0] = '\0';

    FILE* fGGA = NULL;
    FILE* fKML = NULL;

    unsigned long numofpos = 0;

    rtcm_buff_t rtcm_buffer = { 0 };
    int data = 0;

    std::vector<msg_t> v_msg;

    std::vector<std::string> log_msg;

    std::vector<gap_t> log_gap;

    double lastEpoch = 0.0;
    unsigned long numofepoch = 0;
    unsigned long numofgap[6] = { 0 };
    unsigned long numofcrc = 0;
    unsigned long numofmsg = 0;

    while (fRTCM != NULL && !feof(fRTCM))
    {
        if ((data = fgetc(fRTCM)) == EOF) break;
        int ret = input_rtcm3_type(&rtcm_buffer, (unsigned char)data);
        if (rtcm_buffer.type > 0)
        {
            ++numofmsg;
            //printf("%4i,%i,%4i,%4i,%4i,%10.3f,%c%03i\r\n", rtcm_buffer.type, rtcm_buffer.crc, rtcm_buffer.len, rtcm_buffer.slen, rtcm_buffer.wk, rtcm_buffer.tow, rtcm_buffer.sys, rtcm_buffer.prn);
            if (rtcm_buffer.crc == 0)
            {
                bool is_new_msg = true;
                for (std::vector<msg_t>::iterator p_msg = v_msg.begin(); p_msg != v_msg.end(); ++p_msg)
                {
                    if (p_msg->type == rtcm_buffer.type)
                    {
                        ++p_msg->count;
                        p_msg->time = rtcm_buffer.tow;
                        is_new_msg = false;
                    }
                }
                if (is_new_msg)
                {
                    if (v_msg.size() == 0)
                        fLOG = fopen("log.txt", "a+");
                    msg_t msg = { 0 };
                    msg.time = rtcm_buffer.tow;
                    msg.type = rtcm_buffer.type;
                    v_msg.push_back(msg);
                }
            }
            else
            {
                ++numofcrc;
            }
        }
        if (ret == 1) {
            ++numofepoch;
            if (numofepoch > 1)
            {
                double dt = rtcm_buffer.tow - lastEpoch;
                while (dt < -WEEK_SECOND_HALF)
                {
                    dt += WEEK_SECOND_FULL;
                }
                if (dt > 1.5)
                {
                    gap_t gap = { 0 };
                    gap.start_time = lastEpoch;
                    gap.end_time = rtcm_buffer.tow;
                    log_gap.push_back(gap);
                    if (dt < 4.5)
                        ++numofgap[0];
                    else if (dt < 9.5)
                        ++numofgap[1];
                    else if (dt < 14.5)
                        ++numofgap[2];
                    else if (dt < 29.5)
                        ++numofgap[3];
                    else if (dt < 59.5)
                        ++numofgap[4];
                    else
                        ++numofgap[5];
                    //printf("%10.3f,%10.3f,%u,%u,%u,%u,%u,%u,%s\r\n", gap.start_time, dt, numofepoch, numofgap[0], numofgap[1], numofgap[2], numofgap[3], numofgap[4]);
                    if (fLOG)
                    {
                        fprintf(fLOG, "%10.3f,%10.3f,%7u,%7u(5s),%7u(10s),%7u(15s),%7u(30s),%7u(60s),%7u(>60s),%s,data gap record\r\n", gap.start_time, dt, numofepoch, numofgap[0], numofgap[1], numofgap[2], numofgap[3], numofgap[4], numofgap[5], fname);
                    }
                }
            }
            lastEpoch = rtcm_buffer.tow;
        }
        if (ret == 5 && fabs(rtcm_buffer.pos[0]) > 0.1 && fabs(rtcm_buffer.pos[1]) > 0.1 && fabs(rtcm_buffer.pos[2]) > 0.1)
        {
            if (numofpos == 0)
            {
                sprintf(outfilename, "%s-.kml", fileName);
                fKML = fopen(outfilename, "w");
                print_kml_heder(fKML);
                sprintf(outfilename, "%s-.nmea", fileName);
                fGGA = fopen(outfilename, "w");
            }
            ++numofpos;
            double blh[3] = { 0 };
            xyz2blh(rtcm_buffer.pos, blh);
            if (fGGA)
            {
                char gga[255] = { 0 };
                outnmea_gga((unsigned char*)gga, rtcm_buffer.tow, 1, blh, 10, 1.0, 0.0);
                fprintf(fGGA, "%s", gga);
            }
            char sol_status[2] = { 0 };
            print_kml_gga(fKML, blh[0] * 180.0 / PI, blh[1] * 180.0 / PI, blh[2], 1, rtcm_buffer.tow, 0, sol_status);
        }
    }
    if (fLOG)
    {
        for (std::vector<msg_t>::iterator p_msg = v_msg.begin(); p_msg != v_msg.end(); ++p_msg)
        {
            fprintf(fLOG, "%7u(%4i),", p_msg->count, p_msg->type);
        }
        fprintf(fLOG, "%7u,%7u,%7u,%s,total epoch, total msg count, total crc failed msg\r\n", numofepoch, numofmsg, numofcrc, fname);
    }
    if (fRTCM) fclose(fRTCM);
    if (fGGA) fclose(fGGA);
    if (fLOG) fclose(fLOG);
    if (fKML) {
        print_kml_eof(fKML);
        fclose(fKML);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
    }
    else
    {
        rtcm2gga(argv[1]);
    }
    return 0;
}


