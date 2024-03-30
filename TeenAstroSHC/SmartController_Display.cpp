#include "SmartController.h"
#include "SHC_text.h"

#define MY_BORDER_SIZE 1
#define icon_width 16
#define icon_height 16

#define teenastro_width 128
#define teenastro_height 68


static unsigned char shift_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x40, 0x02, 0x20, 0x04, 0x10, 0x08,
   0x08, 0x10, 0x04, 0x20, 0x1c, 0x38, 0x10, 0x08, 0x10, 0x08, 0x10, 0x08,
   0x10, 0x08, 0x10, 0x08, 0xf0, 0x0f, 0x00, 0x00 };

static unsigned char wifi_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x20, 0x40, 0x4e, 0x00, 0x11,
    0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0xfe, 0x7f, 0x02, 0x40, 0xda, 0x5f,
    0xda, 0x5f, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00 };

static unsigned char wifi_not_connected_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x04, 0x00, 0x04, 0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40,
    0x02, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00 };

static unsigned char align1_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x50, 0x02, 0x18, 0x04, 0x10, 0x08,
    0x10, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00 };

static unsigned char align2_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x48, 0x02, 0x14, 0x04, 0x10, 0x08,
    0x08, 0x10, 0x04, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char align3_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x1c, 0x00, 0x00, 0x00,
    0xc0, 0x03, 0xc0, 0x00, 0x40, 0x01, 0x40, 0x02, 0x1c, 0x04, 0x10, 0x08,
    0x18, 0x10, 0x10, 0x20, 0x1c, 0x00, 0x00, 0x00 };

static unsigned char home_bits[] U8X8_PROGMEM = {
    0x00, 0x02, 0x00, 0x07, 0x80, 0x0f, 0xc0, 0x1f, 0x80, 0x3f, 0x00, 0x7f,
    0x00, 0x7e, 0x00, 0x7f, 0x80, 0xfb, 0xc0, 0xc1, 0xe0, 0x01, 0xbc, 0x49,
    0x9e, 0x49, 0x9e, 0x79, 0x8c, 0x49, 0x80, 0x49 };

static unsigned char parked_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0xfe, 0x7f, 0x02, 0x40, 0x02, 0x40, 0xe2, 0x43, 0x62, 0x46,
    0x62, 0x46, 0x62, 0x46, 0xe2, 0x43, 0x62, 0x40, 0x62, 0x40, 0x62, 0x40,
    0x62, 0x40, 0x02, 0x40, 0xfe, 0x7f, 0x00, 0x00 };

static unsigned char parking_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0xf9, 0x80, 0x99, 0x81, 0x99, 0x81,
    0x99, 0x81, 0xf9, 0x80, 0x19, 0x80, 0x99, 0x84, 0x99, 0x8d, 0x99, 0x9f,
    0x81, 0x8d, 0x81, 0x84, 0x01, 0x80, 0xff, 0xff };

static unsigned char parkingFailed_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0xf9, 0x90, 0x99, 0x91, 0x99, 0x91,
    0x99, 0x91, 0xf9, 0x90, 0x19, 0x90, 0xd9, 0x93, 0x59, 0x90, 0xd9, 0x91,
    0x41, 0x80, 0x41, 0x90, 0x01, 0x80, 0xff, 0xff };

static unsigned char guiding_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x80, 0x01, 0x80, 0x01, 0xc0, 0x03, 0x20, 0x04, 0x10, 0x08,
    0x08, 0x10, 0x8e, 0x71, 0x8e, 0x71, 0x08, 0x10, 0x10, 0x08, 0x20, 0x04,
    0xc0, 0x03, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00 };

static unsigned char guiding_W_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x70,
    0x00, 0x78, 0x00, 0x7c, 0x00, 0x7c, 0x00, 0x78, 0x00, 0x70, 0x00, 0x60,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char guiding_N_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xc0, 0x03,
    0xe0, 0x07, 0xf0, 0x0f, 0xf0, 0x0f, 0x00, 0x00 };

static unsigned char guiding_S_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0xf0, 0x0f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char guiding_E_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x0e, 0x00,
    0x1e, 0x00, 0x3e, 0x00, 0x3e, 0x00, 0x1e, 0x00, 0x0e, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char guiding__bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0xf0, 0x0f, 0x10, 0x08, 0x20, 0x04, 0x46, 0x62, 0x8a, 0x51,
    0x12, 0x48, 0x22, 0x44, 0x22, 0x44, 0x12, 0x48, 0x8a, 0x51, 0x46, 0x62,
    0x20, 0x04, 0x10, 0x08, 0xf0, 0x0f, 0x00, 0x00 };

static unsigned char no_tracking_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e,
    0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7c, 0x3e,
    0x7c, 0x3e, 0x7c, 0x3e, 0x38, 0x1c, 0x00, 0x00 };

static unsigned char tracking_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x06, 0x00, 0x0e, 0x00, 0x1e, 0x00,
    0x3e, 0x00, 0x7e, 0x00, 0xfe, 0x00, 0x7e, 0x00, 0x3e, 0x00, 0x1e, 0x00,
    0x0e, 0x00, 0x06, 0x00, 0x02, 0x00, 0x00, 0x00 };

static unsigned char tracking_1_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x00, 0x10, 0x00, 0x18, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10,
   0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char tracking_2_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x20, 0x00, 0x18, 0x00, 0x04,
   0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char tracking_target_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x10, 0x00, 0x10, 0x00, 0x10,
   0x00, 0x10, 0x00, 0x10, 0x00, 0x10, 0x00, 0x00 };

static unsigned char tracking_moon_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x0e, 0x00, 0x0e,
    0x00, 0x0e, 0x00, 0x0e, 0x00, 0x3c, 0x00, 0x00 };

static unsigned char tracking_star_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x04, 0x00, 0x04, 0x00, 0x18,
    0x00, 0x20, 0x00, 0x20, 0x00, 0x1c, 0x00, 0x00 };

static unsigned char tracking_sun_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x42, 0x00, 0x5a,
    0x00, 0x5a, 0x00, 0x42, 0x00, 0x3c, 0x00, 0x00 };

static unsigned char sleewing_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x06, 0x03, 0x0e, 0x07, 0x1e, 0x0f,
    0x3e, 0x1f, 0x7e, 0x3f, 0xfe, 0x7f, 0x7e, 0x3f, 0x3e, 0x1f, 0x1e, 0x0f,
    0x0e, 0x07, 0x06, 0x03, 0x02, 0x01, 0x00, 0x00 };

static unsigned char W_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x10, 0x04, 0x08, 0x08, 0x24, 0x12,
    0x22, 0x22, 0x22, 0x22, 0xa2, 0x22, 0xa2, 0x22, 0x42, 0x21, 0x44, 0x11,
    0x08, 0x08, 0x10, 0x04, 0xe0, 0x03, 0x00, 0x00 };

static unsigned char E_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x10, 0x04, 0x08, 0x08, 0xe4, 0x13,
    0x22, 0x20, 0x22, 0x20, 0xe2, 0x21, 0x22, 0x20, 0x22, 0x20, 0xe4, 0x13,
    0x08, 0x08, 0x10, 0x04, 0xe0, 0x03, 0x00, 0x00 };

static unsigned char ErrA1_bits[] U8X8_PROGMEM = {
   0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
   0x2e, 0xb1, 0x00, 0xb0, 0x00, 0xb0, 0x1c, 0xb2, 0x22, 0xb3, 0xa2, 0xb2,
   0x3e, 0x82, 0x22, 0xb2, 0x22, 0xb2, 0x00, 0x80 };

static unsigned char ErrA2_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
   0x2e, 0xb1, 0x00, 0xb0, 0x00, 0xb0, 0x1c, 0xb0, 0xa2, 0xb3, 0x22, 0xb2,
   0xbe, 0x83, 0xa2, 0xb0, 0xa2, 0xb3, 0x00, 0x80 };

static unsigned char ErrHo_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
    0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x22, 0xb0, 0x22, 0xb3, 0xbe, 0xb4,
    0xa2, 0x84, 0xa2, 0xb4, 0x22, 0xb3, 0x00, 0x80 };

static unsigned char ErrMe_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
    0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x36, 0xb0, 0xaa, 0xb3, 0xa2, 0xb2,
    0xa2, 0x83, 0xa2, 0xb0, 0xa2, 0xb3, 0x00, 0x80 };

static unsigned char ErrMf_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
    0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x36, 0xb0, 0xaa, 0xb3, 0xa2, 0xb0,
    0xa2, 0x81, 0xa2, 0xb0, 0xa2, 0xb0, 0x00, 0x80 };

static unsigned char ErrUp_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0x00, 0x80, 0x0e, 0xb0, 0x02, 0xb0, 0x66, 0xb3, 0x22, 0xb1,
    0x2e, 0xb1, 0x00, 0xb0, 0x22, 0xb0, 0x22, 0xb0, 0xa2, 0xb3, 0xa2, 0xb2,
    0xa2, 0x83, 0xa2, 0xb0, 0x9c, 0xb0, 0x00, 0x80 };

static unsigned char Lock___bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x21,
    0x00, 0x21, 0x80, 0x7f, 0x80, 0x7f, 0x80, 0x7f, 0x80, 0x7f, 0x80, 0x7f,
    0x80, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char Lock_F_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x3e, 0x00, 0x02, 0x00, 0x02, 0x00, 0x1e, 0x00, 0x02, 0x00,
    0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char Lock_T_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x08, 0x00, 0x08, 0x00,
    0x08, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00 };

static unsigned char GNSS_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x10, 0x01, 0x08, 0x01, 0x10, 0x07,
    0xe0, 0x07, 0x80, 0x1f, 0x80, 0x23, 0x2a, 0x42, 0x4a, 0x22, 0x12, 0x14,
    0x64, 0x08, 0x08, 0x00, 0x70, 0x00, 0x00, 0x00 };

static unsigned char GNSSL_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x10, 0x01, 0x08, 0x01, 0x10, 0x07,
   0xe0, 0x07, 0x80, 0x1f, 0x80, 0x23, 0x00, 0x42, 0x84, 0x22, 0x84, 0x14,
   0x84, 0x08, 0x04, 0x00, 0xbc, 0x00, 0x00, 0x00 };

static unsigned char GNSST_bits[] U8X8_PROGMEM = {
   0x00, 0x00, 0x40, 0x00, 0xa0, 0x00, 0x10, 0x01, 0x08, 0x01, 0x10, 0x07,
   0xe0, 0x07, 0x80, 0x1f, 0x80, 0x23, 0x00, 0x42, 0xbe, 0x22, 0x88, 0x14,
   0x88, 0x08, 0x08, 0x00, 0x88, 0x00, 0x00, 0x00 };

static unsigned char GUIDINGSP_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff };

static unsigned char SLOWSP_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff };

static unsigned char MEDIUMSP_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff };

static unsigned char FASTSP_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff };

static unsigned char MAXSP_bits[] U8X8_PROGMEM = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff };

static unsigned char Spiral_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0xfc, 0x3f, 0xfe, 0x7f, 0x06, 0x60, 0x06, 0x60, 0xc6, 0x63,
    0xe6, 0x67, 0x66, 0x66, 0x66, 0x66, 0x06, 0x66, 0x06, 0x66, 0xfe, 0x67,
    0xfc, 0x63, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00 };

static unsigned char Aligned_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x02, 0x40, 0x04, 0x20, 0x28, 0x14, 0x30, 0x0c, 0x38, 0x1c,
    0x80, 0x01, 0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x38, 0x1c, 0x30, 0x0c,
    0x28, 0x14, 0x04, 0x20, 0x02, 0x40, 0x00, 0x00 };

static const unsigned char teenastro_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x01, 0x00, 0xfc, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfe, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xfe, 0x0f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xff, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x7e, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x7f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x0f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9c, 0x7f, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xfe,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0xf8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0f, 0xe0, 0x07, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1f, 0xc0,
    0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xc0, 0x1e, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x1e, 0x00, 0xfc, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x3c, 0x00,
    0xf0, 0x81, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x60, 0x2c, 0x00, 0x80, 0xc7, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x68, 0x00, 0x00, 0xce, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x00,
    0x00, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x10, 0xd8, 0x00, 0x00, 0xf0, 0x7f, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xd8, 0x00, 0x00, 0xf0, 0x7f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x98, 0x01,
    0x00, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0c, 0x90, 0x01, 0x00, 0xf0, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x30, 0x01, 0x00, 0xf0, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x30, 0x03,
    0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x30, 0x07, 0x00, 0x80, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x06, 0x00, 0x80, 0x7f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x20, 0x04,
    0x00, 0x80, 0xff, 0x0f, 0xfe, 0x01, 0x00, 0x00, 0x1c, 0x00, 0x02, 0x00,
    0x80, 0x01, 0x20, 0x0c, 0x00, 0xc0, 0xff, 0x3f, 0xfe, 0x01, 0x00, 0x00,
    0x1c, 0x00, 0x03, 0x00, 0x80, 0x00, 0x20, 0x08, 0x00, 0xe0, 0xff, 0xff,
    0x30, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x03, 0x00, 0xc0, 0x00, 0x20, 0x18,
    0x00, 0xf0, 0xff, 0xff, 0x30, 0x38, 0x1c, 0x1b, 0x3e, 0x9c, 0xb7, 0x18,
    0x40, 0x00, 0x60, 0x30, 0x00, 0xfc, 0xff, 0xff, 0x30, 0x6c, 0x36, 0x3f,
    0x36, 0xbe, 0xf7, 0x3c, 0x60, 0x00, 0x40, 0x30, 0x80, 0xff, 0xff, 0xff,
    0x30, 0x6c, 0x36, 0x33, 0x36, 0x26, 0x33, 0x66, 0x20, 0x00, 0x40, 0x20,
    0xc0, 0x8f, 0xff, 0xff, 0x30, 0x7c, 0x3e, 0x33, 0x63, 0x1e, 0x33, 0x66,
    0x20, 0x00, 0x40, 0x60, 0xc0, 0x07, 0xff, 0xff, 0x30, 0x0c, 0x06, 0x33,
    0x7f, 0x38, 0x33, 0x66, 0x30, 0x00, 0x40, 0x40, 0xc0, 0x1f, 0xff, 0xff,
    0x30, 0x6c, 0x36, 0x33, 0x7f, 0x32, 0x33, 0x66, 0x10, 0x00, 0xc0, 0xc0,
    0xc0, 0x7f, 0xff, 0xff, 0x30, 0x6c, 0x36, 0xb3, 0xc1, 0x3e, 0x37, 0x3c,
    0x18, 0x00, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0x30, 0x38, 0x1c, 0xb3,
    0xc1, 0x1c, 0x36, 0x1c, 0x0c, 0x00, 0xc0, 0x80, 0xc1, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0xc0, 0x81,
    0xc1, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0e, 0x00, 0xc0, 0x81, 0xf3, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x80, 0xc3, 0xf3, 0xff, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x80, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void SmartHandController::drawIntro()
{
  display->firstPage();
  do
  {
    display->drawXBMP(0, 0, teenastro_width, teenastro_height, teenastro_bits);
  } while (display->nextPage());
  delay(1500);
}

void SmartHandController::updateMainDisplay(PAGES page)
{
  u8g2_t* u8g2 = display->getU8g2();
  display->setFont(u8g2_font_helvR12_te);
  u8g2_uint_t line_height = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2) + MY_BORDER_SIZE;
  u8g2_uint_t step1 = u8g2_GetUTF8Width(u8g2, "44");
  u8g2_uint_t step2 = u8g2_GetUTF8Width(u8g2, "4") + 1;
  ta_MountStatus.removeLastConnectionFailure();
  ta_MountStatus.updateMount();
  if (ta_MountStatus.isAligning())
    page = P_ALIGN;

  if (ta_MountStatus.hasInfoMount() && page == P_ALIGN)
  {
    TeenAstroMountStatus::TrackState curT = ta_MountStatus.getTrackingState();
    if (curT != TeenAstroMountStatus::TRK_SLEWING && ta_MountStatus.isAlignSlew())
    {
      ta_MountStatus.nextStepAlign();
    }
  }
  else if (page == P_RADEC && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateRaDec();
  }
  else if (page == P_HADEC && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateHaDec();
  }
  else if (page == P_ALTAZ && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateAzAlt();
  }
  else if (page == P_PUSH && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updatePush();
  }
  else if (page == P_TIME && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateTime();
  }
  else if (page == P_FOCUSER && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateFocuser();
  }
  else if (page == P_AXIS_STEP && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateAxisStep();
  }
  else if (page == P_AXIS_DEG && !ta_MountStatus.isPulseGuiding())
  {
    ta_MountStatus.updateAxisDeg();
  }
  u8g2_FirstPage(u8g2);

  do
  {
    u8g2_uint_t x = u8g2_GetDisplayWidth(u8g2);
    u8g2_uint_t xl = 0;
    int k = 0;
    if (buttonPad.isWifiOn())
    {
      buttonPad.isWifiRunning() ? display->drawXBMP(xl, 0, icon_width, icon_height, wifi_bits) : display->drawXBMP(0, 0, icon_width, icon_height, wifi_not_connected_bits);
      xl += icon_width + 1;
    }
    if (ta_MountStatus.hasInfoMount())
    {
      TeenAstroMountStatus::ParkState curP = ta_MountStatus.getParkState();
      TeenAstroMountStatus::TrackState curT = ta_MountStatus.getTrackingState();
      TeenAstroMountStatus::SiderealMode currSM = ta_MountStatus.getSiderealMode();
      TeenAstroMountStatus::PierState curPi = ta_MountStatus.getPierState();
      TeenAstroMountStatus::RateCompensation curC = ta_MountStatus.getRateCompensation();
      if (ta_MountStatus.hasGNSSBoard() && (ta_MountStatus.atHome() || curP == TeenAstroMountStatus::PRK_PARKED))
      {
        if (ta_MountStatus.isGNSSValid())
        {
          if (!ta_MountStatus.isGNSSTimeSync())
          {
            display->drawXBMP(xl, 0, icon_width, icon_height, GNSST_bits);
            xl += icon_width + 1;
          }
          if (!ta_MountStatus.isGNSSLocationSync())
          {
            display->drawXBMP(xl, 0, icon_width, icon_height, GNSSL_bits);
            xl += icon_width + 1;
          }
          if (ta_MountStatus.isGNSSLocationSync() && ta_MountStatus.isGNSSTimeSync())
          {
            display->drawXBMP(xl, 0, icon_width, icon_height, GNSS_bits);
            xl += icon_width + 1;
          }
        }
      }

      if (ta_MountStatus.motorsEnable())
      {
        switch (ta_MountStatus.getGuidingRate())
        {
        case (TeenAstroMountStatus::GuidingRate::GUIDING):
          display->drawXBMP(xl, 0, icon_width, icon_height, GUIDINGSP_bits);
          break;
        case (TeenAstroMountStatus::GuidingRate::SLOW):
          display->drawXBMP(xl, 0, icon_width, icon_height, SLOWSP_bits);
          break;
        case (TeenAstroMountStatus::GuidingRate::MEDIUM):
          display->drawXBMP(xl, 0, icon_width, icon_height, MEDIUMSP_bits);
          break;
        case (TeenAstroMountStatus::GuidingRate::FAST):
          display->drawXBMP(xl, 0, icon_width, icon_height, FASTSP_bits);
          break;
        case (TeenAstroMountStatus::GuidingRate::MAX):
          display->drawXBMP(xl, 0, icon_width, icon_height, MAXSP_bits);
          break;
        default:
          break;
        }
      }

      if (curP == TeenAstroMountStatus::PRK_PARKED)
      {
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parked_bits);
        x -= icon_width + 1;
      }
      else if (curP == TeenAstroMountStatus::PRK_PARKING)
      {
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parking_bits);
        x -= icon_width + 1;
      }
      else if (ta_MountStatus.atHome())
      {
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, home_bits);
        x -= icon_width + 1;
      }
      else
      {
        if (ta_MountStatus.motorsEnable())
        {
          if (curT == TeenAstroMountStatus::TRK_SLEWING)
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, sleewing_bits);
            x -= icon_width + 1;
          }
          else if (curT == TeenAstroMountStatus::TRK_ON)
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_bits);
            display->setBitmapMode(1);
            switch (currSM)
            {
            case TeenAstroMountStatus::SID_UNKNOWN:
              break;
            case TeenAstroMountStatus::SID_STAR:
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_star_bits);
              break;
            case TeenAstroMountStatus::SID_SUN:
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_sun_bits);
              break;
            case TeenAstroMountStatus::SID_MOON:
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_moon_bits);
              break;
            case TeenAstroMountStatus::SID_TARGET:
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_target_bits);
              break;
            default:
              break;
            }
            switch (curC)
            {
            case TeenAstroMountStatus::RC_RA:
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_1_bits);
              break;
            case TeenAstroMountStatus::RC_BOTH:
              display->drawXBMP(x - icon_width, 0, icon_width, icon_height, tracking_2_bits);
              break;
            default:
              break;
            }

            display->setBitmapMode(0);

            x -= icon_width + 1;
          }
          else if (curT == TeenAstroMountStatus::TRK_OFF)
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, no_tracking_bits);
            x -= icon_width + 1;
          }
        }
        if (curP == TeenAstroMountStatus::PRK_FAILED)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, parkingFailed_bits);
          x -= icon_width + 1;
        }

        if (curPi == TeenAstroMountStatus::PIER_E)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, E_bits);
          x -= icon_width + 1;
        }
        else if (curPi == TeenAstroMountStatus::PIER_W)
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, W_bits);
          x -= icon_width + 1;
        }

        if (ta_MountStatus.isAligning())
        {
          if (ta_MountStatus.getAlignMode() == TeenAstroMountStatus::ALIM_ONE)
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align1_bits);
          else if (ta_MountStatus.getAlignMode() == TeenAstroMountStatus::ALIM_TWO)
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align2_bits);
          else if (ta_MountStatus.getAlignMode() == TeenAstroMountStatus::ALIM_THREE)
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, align3_bits);
          x -= icon_width + 1;
        }
        else if (ta_MountStatus.isSpiralRunning())
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, Spiral_bits);
          x -= icon_width + 1;
        }
        else if (ta_MountStatus.isPulseGuiding())
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding__bits);
          display->setBitmapMode(1);
          if (ta_MountStatus.isGuidingN())
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding_N_bits);
          }
          else if (ta_MountStatus.isGuidingS())
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding_S_bits);
          }
          if (ta_MountStatus.isGuidingE())
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding_E_bits);
          }
          else if (ta_MountStatus.isGuidingW())
          {
            display->drawXBMP(x - icon_width, 0, icon_width, icon_height, guiding_W_bits);
          }
          display->setBitmapMode(0);
          x -= icon_width + 1;
        }
        else if (ta_MountStatus.isAligned())
        {
          display->drawXBMP(x - icon_width, 0, icon_width, icon_height, Aligned_bits);
          x -= icon_width + 1;
        }
      }
      switch (ta_MountStatus.getError())
      {
      case TeenAstroMountStatus::ERR_MOTOR_FAULT:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMf_bits);
        x -= icon_width + 1;
        break;
      case TeenAstroMountStatus::ERR_ALT:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrHo_bits);
        x -= icon_width + 1;
        break;
      case TeenAstroMountStatus::ERR_LIMIT_A1:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrA1_bits);
        x -= icon_width + 1;
        break;
      case TeenAstroMountStatus::ERR_LIMIT_A2:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrA2_bits);
        x -= icon_width + 1;
        break;
      case TeenAstroMountStatus::ERR_UNDER_POLE:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrUp_bits);
        x -= icon_width + 1;
        break;
      case TeenAstroMountStatus::ERR_MERIDIAN:
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, ErrMe_bits);
        x -= icon_width + 1;
        break;
      default:
        break;
      }
    }
    if (buttonPad.shiftPressed())
    {
      display->drawXBMP(xl, 0, icon_width, icon_height, shift_bits);
      xl += icon_width + 1;
    }
    if (focuserlocked || telescoplocked)
    {
      display->drawXBMP(x - icon_width, 0, icon_width, icon_height, Lock___bits);
      display->setBitmapMode(1);
      if (focuserlocked)
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, Lock_F_bits);
      if (telescoplocked)
        display->drawXBMP(x - icon_width, 0, icon_width, icon_height, Lock_T_bits);
      display->setBitmapMode(0);
      x -= icon_width + 1;
    }
    if (page == P_RADEC)
    {
      if (ta_MountStatus.hasInfoRa() && ta_MountStatus.hasInfoDec())
      {
        u8g2_uint_t y = 36;
        x = u8g2_GetDisplayWidth(u8g2);
        display->drawRA(x, y, ta_MountStatus.getRa());
        u8g2_DrawUTF8(u8g2, 0, y, "RA");
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, "Dec");
        display->drawDec(x, y, ta_MountStatus.getDec());
      }
    }
    else if (page == P_HADEC)
    {
      if (ta_MountStatus.hasInfoHa() && ta_MountStatus.hasInfoDec())
      {
        u8g2_uint_t y = 36;
        x = u8g2_GetDisplayWidth(u8g2);
        display->drawRA(x, y, ta_MountStatus.getHa());
        u8g2_DrawUTF8(u8g2, 0, y, "HA");
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, "Dec");
        display->drawDec(x, y, ta_MountStatus.getDec());
      }
    }
    else if (page == P_ALTAZ)
    {
      if (ta_MountStatus.hasInfoAz() && ta_MountStatus.hasInfoAlt())
      {
        u8g2_uint_t y = 36;
        u8g2_uint_t startpos = u8g2_GetUTF8Width(u8g2, "123456");
        x = startpos;
        x = u8g2_GetDisplayWidth(u8g2);
        u8g2_DrawUTF8(u8g2, 0, y, "Az.");
        display->drawAz(x, y, ta_MountStatus.getAz());
        y += line_height + 4;
        x = startpos;
        x = u8g2_GetDisplayWidth(u8g2);
        display->drawDec(x, y, ta_MountStatus.getAlt());
        u8g2_DrawUTF8(u8g2, 0, y, "Alt.");
      }
    }
    else if (page == P_PUSH)
    {
      if (ta_MountStatus.hasInfoPush())
      {
        display->setFont(u8g2_font_courB18_tn);
        u8g2_uint_t y = 39;
        x = u8g2_GetDisplayWidth(u8g2);
        const char* txt1 = ta_MountStatus.GetPushA1();
        const char* txt2 = ta_MountStatus.GetPushA2();
        u8g2_DrawUTF8(u8g2, 20, y, ta_MountStatus.GetPushA1());
        y += 22;
        u8g2_DrawUTF8(u8g2, 20, y, ta_MountStatus.GetPushA2());
        display->setFont(u8g2_font_helvR12_te);
      }
    }
    else if (page == P_TIME)
    {
      if (ta_MountStatus.hasInfoUTC() && ta_MountStatus.hasInfoSidereal())
      {
        u8g2_uint_t y = 36;
        x = u8g2_GetDisplayWidth(u8g2);
        u8g2_DrawUTF8(u8g2, 0, y, "UTC");
        display->drawRA(x, y, ta_MountStatus.getUTC());
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, "LST");
        display->drawRA(x, y, ta_MountStatus.getSidereal());
      }
    }
    else if (page == P_FOCUSER)
    {
      u8g2_uint_t y = 36;
      if (ta_MountStatus.hasInfoFocuser())
      {
        display->drawFoc(y, line_height, ta_MountStatus.getFocuser());
      }
      else
      {
        u8g2_DrawUTF8(u8g2, 0, y, T_FOCUSER);
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, T_NOT_CONNECTED);
      }
    }
    else if (page == P_AXIS_STEP)
    {
      u8g2_uint_t y = 36;
      if (ta_MountStatus.hasInfoAxis1Step())
      {
        x = u8g2_GetDisplayWidth(u8g2);
        u8g2_DrawUTF8(u8g2, 0, y, ta_MountStatus.getAxis1Step());
        y += line_height + 4;
        u8g2_DrawUTF8(u8g2, 0, y, ta_MountStatus.getAxis2Step());
      }
    }
    else if (page == P_AXIS_DEG)
    {
      display->setFont(u8g2_font_helvR08_te);
      line_height = u8g2_GetAscent(u8g2) - u8g2_GetDescent(u8g2) + MY_BORDER_SIZE;
      u8g2_uint_t y = 26;
      x = u8g2_GetDisplayWidth(u8g2);
      u8g2_DrawUTF8(u8g2, 0, y, "Ax1.");
      display->drawIDeg(x, y, ta_MountStatus.getAxis1Deg());
      y += line_height;
      u8g2_DrawUTF8(u8g2, 0, y, "Ax2.");
      display->drawIDeg(x, y, ta_MountStatus.getAxis2Deg());
      y += line_height;
      if (ta_MountStatus.encodersEnable())
      {
        u8g2_DrawUTF8(u8g2, 0, y, "Ax1E.");
        display->drawIDeg(x, y, ta_MountStatus.getAxis1EDeg());
        y += line_height;
        u8g2_DrawUTF8(u8g2, 0, y, "Ax2E.");
        display->drawIDeg(x, y, ta_MountStatus.getAxis2EDeg());
      }
      else
      {
        u8g2_DrawUTF8(u8g2, 0, y, "Ax1.");
        display->drawIDeg(x, y, ta_MountStatus.getAxis1Degc());
        y += line_height;
        u8g2_DrawUTF8(u8g2, 0, y, "Ax2.");
        display->drawIDeg(x, y, ta_MountStatus.getAxis2Degc());
        y += line_height;
      }

      display->setFont(u8g2_font_helvR12_te);
    }

    else if (page == P_ALIGN)
    {
      u8g2_uint_t y = 36;
      static char text1[29] = T_SLEWINGTO " " T_STAR;
      static char text2[28] = T_RECENTER " " T_STAR;
      if (ta_MountStatus.isAlignSlew())
        u8g2_DrawUTF8(u8g2, 0, y, text1);
      else if (ta_MountStatus.isAlignRecenter())
        u8g2_DrawUTF8(u8g2, 0, y, text2);

      y += line_height + 4;
      if (cat_mgr.objectNameStr()[0] != 0)
      {
        u8g2_DrawUTF8(u8g2, 0, y, cat_mgr.objectNameStr());
      }
      else
      {
        u8g2_SetFont(u8g2, u8g2_font_unifont_t_greek);
        u8g2_DrawGlyph(u8g2, 0, y, 945 + cat_mgr.bayerFlam());
        const uint8_t* myfont = u8g2->font;
        u8g2_SetFont(u8g2, myfont);
        u8g2_DrawUTF8(u8g2, 16, y, cat_mgr.constellationStr());
      }
    }
  } while (u8g2_NextPage(u8g2));
  lastpageupdate = millis();
}
