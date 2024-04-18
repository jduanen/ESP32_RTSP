
#pragma once

// definitions for the XIAO ESP32-S3 SENSE board
camera_config_t xiaoCam_config {
    .pin_pwdn = -1,
    .pin_reset = -1,

    .pin_xclk = 10,

    .pin_sscb_sda = 40,
    .pin_sscb_scl = 39,

    .pin_d7 = 48,
    .pin_d6 = 11,
    .pin_d5 = 12,
    .pin_d4 = 14,
    .pin_d3 = 16,
    .pin_d2 = 18,
    .pin_d1 = 17,
    .pin_d0 = 15,
    .pin_vsync = 38,
    .pin_href = 47,
    .pin_pclk = 13,
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,
    .pixel_format = PIXFORMAT_JPEG,
    // .frame_size = FRAMESIZE_UXGA, // needs 234K of framebuffer space
    // .frame_size = FRAMESIZE_SXGA, // needs 160K for framebuffer
    // .frame_size = FRAMESIZE_XGA, // needs 96K or even smaller FRAMESIZE_SVGA - can work if using only 1 fb
    .frame_size = FRAMESIZE_SVGA,
    .jpeg_quality = 12,               //0-63 lower numbers are higher quality
    .fb_count = 2 // if more than one i2s runs in continous mode.  Use only with jpeg
};
