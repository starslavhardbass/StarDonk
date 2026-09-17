#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../JuceLibraryCode/BinaryData.h"

namespace
{
static const unsigned char semechkiPngData[] =
{
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
    0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x49, 0x08, 0x06, 0x00, 0x00, 0x00, 0x7b, 0x2f, 0xa5,
    0x8e, 0x00, 0x00, 0x14, 0x71, 0x49, 0x44, 0x41, 0x54, 0x78, 0xda, 0xed, 0x5b, 0x69, 0x90, 0x5c,
    0x57, 0x75, 0xfe, 0xce, 0xbd, 0xef, 0xf5, 0x7b, 0xbd, 0xcd, 0x2a, 0x69, 0xb4, 0x5a, 0xb2, 0x25,
    0xdb, 0xd8, 0x72, 0x82, 0xf1, 0x26, 0x30, 0x8b, 0x67, 0x20, 0x36, 0xd8, 0x18, 0x70, 0xc5, 0x68,
    0x52, 0x0e, 0xa9, 0xe0, 0x18, 0x0c, 0x95, 0x82, 0x24, 0x24, 0x40, 0x8a, 0xc5, 0x89, 0x46, 0x84,
    0x84, 0x90, 0xa2, 0x20, 0x3f, 0x00, 0x07, 0x97, 0xc1, 0x09, 0x06, 0x5c, 0xb4, 0x0c, 0x06, 0xe3,
    0x78, 0x25, 0x8c, 0x22, 0x8c, 0xb1, 0x2d, 0x1b, 0x63, 0x2d, 0x16, 0x96, 0x2d, 0x59, 0xd6, 0x62,
    0x69, 0xb6, 0xde, 0xfb, 0xad, 0xf7, 0xde, 0x93, 0x1f, 0xaf, 0xbb, 0x35, 0xa3, 0x59, 0x25, 0x39,
    0x09, 0xa9, 0x4a, 0x57, 0xcd, 0x9f, 0x99, 0xee, 0xd7, 0xe7, 0x3b, 0xe7, 0x3b, 0xe7, 0x7c, 0xe7,
    0xdc, 0x3b, 0xc0, 0xff, 0xbf, 0x66, 0x7f, 0x31, 0x33, 0x31, 0x17, 0x24, 0x33, 0xcb, 0x4d, 0x9b,
    0x36, 0x89, 0xff, 0x5b, 0xc6, 0x17, 0x0a, 0x72, 0x3a, 0xa0, 0xe9, 0xbf, 0xfb, 0xad, 0x36, 0x9e,
    0x99, 0x1d, 0xbf, 0xbe, 0xf7, 0xaa, 0xea, 0xc8, 0x9e, 0xeb, 0xc7, 0x0f, 0xed, 0x5d, 0x99, 0xfc,
    0x0e, 0xf4, 0xdb, 0x4e, 0x9b, 0xa6, 0xf1, 0xc1, 0xbb, 0x6e, 0xff, 0xc6, 0x97, 0x77, 0xdf, 0x73,
    0xd7, 0xad, 0xcc, 0xd1, 0x41, 0xae, 0x1e, 0x7b, 0xb6, 0x5c, 0x3c, 0xf6, 0xeb, 0xbf, 0x05, 0x80,
    0x42, 0xa1, 0x20, 0x7f, 0x1b, 0x81, 0x50, 0xdb, 0x78, 0x75, 0xe8, 0x33, 0x0f, 0xde, 0x7b, 0x27,
    0xaf, 0x5b, 0x77, 0x36, 0x7f, 0xf9, 0x0b, 0xb7, 0x28, 0x6f, 0x7c, 0x87, 0x1a, 0x7b, 0xf9, 0x71,
    0xf6, 0xc6, 0x77, 0xf0, 0x33, 0x4f, 0x3f, 0xfc, 0x39, 0x00, 0x18, 0x1e, 0x1e, 0xb6, 0x5e, 0x8d,
    0x2f, 0xb5, 0x5e, 0x3d, 0xcf, 0x0f, 0x4b, 0x22, 0x52, 0x1c, 0xee, 0xfb, 0xbb, 0x5f, 0x3f, 0xf9,
    0xec, 0x2d, 0x9f, 0xfc, 0xec, 0x3f, 0x6a, 0xcb, 0x4e, 0x51, 0x77, 0x77, 0x97, 0x2c, 0x95, 0xaa,
    0x38, 0x3a, 0x3e, 0x61, 0x96, 0xf4, 0x74, 0xe9, 0x5f, 0x3e, 0xfa, 0xcb, 0xbf, 0x09, 0x2a, 0x2f,
    0x3c, 0xe5, 0x76, 0x9e, 0x7d, 0x2f, 0x73, 0x41, 0x12, 0x0d, 0xea, 0xff, 0x75, 0x00, 0xcc, 0xc3,
    0x16, 0xd1, 0x80, 0xaa, 0x8f, 0xec, 0xfa, 0xf4, 0xf8, 0x58, 0xf1, 0x96, 0x8f, 0x7c, 0x7c, 0xb3,
    0xca, 0x64, 0xf3, 0x32, 0x97, 0xcb, 0x51, 0x64, 0x14, 0x0c, 0x03, 0x2f, 0xed, 0x3f, 0x2c, 0x2e,
    0xbc, 0xe0, 0x3c, 0x6c, 0x7f, 0x7a, 0x37, 0x77, 0xf7, 0x3c, 0x70, 0x3b, 0x33, 0x5f, 0x48, 0x44,
    0x47, 0x99, 0x59, 0x10, 0x91, 0x39, 0xd5, 0xef, 0x3e, 0xed, 0xd2, 0xc6, 0xc3, 0x89, 0xf1, 0x47,
    0x0e, 0x3c, 0xf6, 0xbe, 0x6c, 0xce, 0xf9, 0x87, 0x4f, 0x7c, 0xea, 0x4b, 0xaa, 0xde, 0x50, 0x72,
    0xd5, 0x8a, 0x15, 0x64, 0x0c, 0x10, 0xf8, 0x01, 0x1c, 0xdb, 0xc2, 0xc8, 0x48, 0x09, 0x52, 0x4a,
    0x71, 0xd6, 0x9a, 0x35, 0xe6, 0x3b, 0x77, 0xfd, 0x64, 0xf1, 0xe8, 0xcb, 0x4f, 0xdd, 0x4a, 0x44,
    0xbc, 0x75, 0xeb, 0xd6, 0xd3, 0xb2, 0x41, 0x9c, 0x6e, 0xb5, 0xa1, 0x81, 0x01, 0xb5, 0xf7, 0xd9,
    0x87, 0x5f, 0xbf, 0xbc, 0x6f, 0xc9, 0x37, 0xbf, 0xf1, 0xad, 0x82, 0x79, 0xfc, 0xe9, 0x1d, 0xf2,
    0xac, 0xb3, 0x56, 0x93, 0xd2, 0x0a, 0x44, 0x40, 0x18, 0x28, 0xa4, 0x1c, 0x1b, 0xd5, 0x5a, 0x1d,
    0x5e, 0xdd, 0xc7, 0xc5, 0x17, 0x9e, 0x27, 0xb7, 0xff, 0xea, 0x39, 0xf5, 0xe3, 0x07, 0x7e, 0xf6,
    0x6e, 0x63, 0x46, 0x6e, 0x1a, 0x18, 0x18, 0x50, 0xad, 0xdc, 0xf9, 0x1f, 0xa5, 0x10, 0x33, 0x0b,
    0x80, 0xcc, 0xae, 0x5d, 0x4f, 0x2c, 0x5d, 0xb9, 0x3c, 0xbb, 0x65, 0xd7, 0x73, 0x7b, 0x9d, 0xdb,
    0xfe, 0xb5, 0x60, 0xd6, 0x9e, 0x79, 0x26, 0xb1, 0x31, 0x00, 0x11, 0x88, 0x00, 0x3f, 0x08, 0x60,
    0x5b, 0x36, 0xfc, 0x28, 0x44, 0xb9, 0x5a, 0xc2, 0x59, 0xab, 0x97, 0x61, 0xe5, 0x8a, 0xa5, 0xe2,
    0xb6, 0x3b, 0xee, 0x36, 0x97, 0x5e, 0xb4, 0xfe, 0x4b, 0xcc, 0xfc, 0x30, 0x80, 0x23, 0xa7, 0x4a,
    0x25, 0x71, 0x8a, 0xc6, 0x13, 0x00, 0x1a, 0x02, 0xd3, 0xd2, 0x2e, 0xeb, 0xbb, 0x29, 0xd7, 0x59,
    0xf9, 0xf7, 0xff, 0x74, 0x9b, 0xce, 0x65, 0xbb, 0x85, 0xe3, 0x38, 0x30, 0x86, 0x93, 0xb2, 0x04,
    0x01, 0x3f, 0x08, 0x61, 0x59, 0x02, 0xac, 0x19, 0xe5, 0x72, 0x15, 0xcb, 0x97, 0xf5, 0xe1, 0xec,
    0xb3, 0x56, 0x89, 0x91, 0x91, 0x71, 0xbe, 0x7b, 0xcb, 0xfd, 0xdd, 0x41, 0x71, 0xf7, 0x57, 0x89,
    0x88, 0x81, 0x53, 0x2b, 0xab, 0xa7, 0x46, 0xa1, 0xad, 0x5b, 0x25, 0x11, 0xe9, 0x9b, 0x5f, 0x78,
    0xe2, 0x2b, 0xbd, 0x2b, 0xfa, 0xde, 0xfa, 0xf5, 0x5b, 0xef, 0x54, 0x2f, 0xee, 0x7b, 0x45, 0x2e,
    0x5a, 0xd4, 0x0b, 0xa5, 0x54, 0xd3, 0x14, 0x06, 0x11, 0x21, 0x08, 0x23, 0x40, 0x10, 0x84, 0x14,
    0x28, 0x55, 0x3c, 0xe4, 0xf2, 0x69, 0xac, 0x3b, 0x73, 0x35, 0xba, 0x3a, 0x3b, 0xe4, 0xdd, 0xf7,
    0x0d, 0xeb, 0x27, 0x9f, 0x7e, 0xee, 0x3d, 0xcc, 0x13, 0x83, 0x44, 0xa4, 0x4f, 0x85, 0x4a, 0xe2,
    0x14, 0xbc, 0x2f, 0x69, 0x60, 0x40, 0xbd, 0xb4, 0xef, 0xf1, 0x9b, 0x56, 0x9c, 0xd1, 0xf7, 0xe7,
    0x4f, 0xfe, 0xfc, 0x09, 0xf5, 0xbd, 0x2d, 0x0f, 0x59, 0xcb, 0x97, 0x2f, 0x83, 0x52, 0xf1, 0xa4,
    0xf7, 0x11, 0x48, 0x00, 0x61, 0x18, 0x03, 0x04, 0x58, 0x96, 0x85, 0x72, 0xa5, 0x0a, 0xb2, 0x25,
    0xd6, 0x9c, 0xb9, 0x0a, 0x69, 0x37, 0x0d, 0x4b, 0x4a, 0xfa, 0xda, 0x6d, 0xdf, 0xe5, 0xf2, 0x91,
    0x83, 0xff, 0xcc, 0x7c, 0xb8, 0x17, 0x00, 0x9f, 0xac, 0x66, 0x12, 0x27, 0xcb, 0x7b, 0x22, 0xd2,
    0x23, 0xfb, 0x77, 0xbe, 0xb6, 0x3b, 0xeb, 0x7c, 0xcd, 0xab, 0xd5, 0xf4, 0x57, 0x6f, 0xbb, 0x4b,
    0xba, 0xe9, 0x2c, 0xa4, 0x24, 0x18, 0x63, 0xc0, 0xcc, 0xad, 0x77, 0x43, 0x10, 0x21, 0x0c, 0x63,
    0xb0, 0x61, 0x38, 0x8e, 0x8d, 0x72, 0xa5, 0x0a, 0x40, 0xe0, 0x8c, 0x95, 0x8b, 0x91, 0xcb, 0x67,
    0xd0, 0xd9, 0x99, 0x17, 0xdb, 0x9f, 0xde, 0x65, 0xfe, 0x63, 0xdb, 0x63, 0xcb, 0x4c, 0xe8, 0x7f,
    0x9e, 0x88, 0xcc, 0xd0, 0xd0, 0x7a, 0xfa, 0x6f, 0x01, 0xc0, 0xcc, 0xb4, 0x65, 0xcb, 0x16, 0x62,
    0x66, 0x9b, 0xc9, 0xff, 0x66, 0x67, 0x77, 0xa7, 0x7b, 0xff, 0xfd, 0xc3, 0xf8, 0xcd, 0xde, 0xc3,
    0xd4, 0xd3, 0xdd, 0x85, 0x28, 0x8a, 0x91, 0x4a, 0xa5, 0xe0, 0xba, 0x6e, 0x1b, 0x04, 0x91, 0x40,
    0x14, 0x45, 0x50, 0x4a, 0x23, 0xed, 0x3a, 0x28, 0x95, 0x3d, 0x40, 0x31, 0x96, 0xf5, 0x2d, 0x46,
    0x67, 0x47, 0x07, 0x2c, 0xcb, 0x46, 0x2e, 0x97, 0x13, 0x5f, 0xff, 0xc6, 0xf7, 0xf4, 0xc8, 0x2b,
    0x13, 0x37, 0x47, 0xe5, 0x83, 0x97, 0x11, 0x0d, 0xea, 0x99, 0x84, 0xe0, 0xab, 0x11, 0x01, 0x31,
    0x38, 0x38, 0xa8, 0xc7, 0x0e, 0x3e, 0xfb, 0x67, 0x4b, 0x16, 0x77, 0x5c, 0x3c, 0x3e, 0x32, 0xa1,
    0xb6, 0xdc, 0x37, 0x2c, 0xbb, 0xba, 0xba, 0xa0, 0x95, 0x82, 0x94, 0x12, 0xbd, 0xbd, 0xbd, 0xc8,
    0x64, 0x32, 0x93, 0x00, 0x10, 0xa2, 0x58, 0x41, 0x45, 0x0a, 0xd9, 0x6c, 0x1a, 0x95, 0x4a, 0x0d,
    0x4a, 0x29, 0x2c, 0xea, 0xed, 0x42, 0x77, 0x57, 0x1e, 0xa9, 0x94, 0x8b, 0x4c, 0xc6, 0xa5, 0x03,
    0x87, 0x47, 0xf1, 0xa3, 0x1f, 0x3f, 0x22, 0x05, 0xa2, 0xaf, 0x9c, 0x6c, 0x32, 0x8b, 0x85, 0x79,
    0x7f, 0x93, 0x00, 0x60, 0x46, 0x46, 0x76, 0x2d, 0x25, 0xe8, 0x5b, 0xd8, 0x18, 0xb3, 0xed, 0xd1,
    0x27, 0xe4, 0xc1, 0xc3, 0x23, 0xc8, 0x67, 0x33, 0x50, 0x5a, 0x23, 0x9f, 0xcf, 0xc3, 0xb2, 0x2c,
    0x34, 0x1a, 0x0d, 0x10, 0x51, 0x1b, 0x40, 0xac, 0x34, 0x82, 0x58, 0x21, 0x9f, 0xcd, 0xa0, 0x5c,
    0xad, 0x22, 0x8e, 0x15, 0x72, 0x99, 0x2c, 0xfa, 0x96, 0xf4, 0x80, 0x04, 0xc1, 0x49, 0xa5, 0xd0,
    0x91, 0xcf, 0xca, 0x3b, 0xee, 0xba, 0x47, 0x8f, 0x4c, 0x8c, 0x5c, 0xee, 0x15, 0xf7, 0xfc, 0x21,
    0x0d, 0x0e, 0x2e, 0x38, 0xa1, 0x17, 0x18, 0x81, 0x21, 0x22, 0x22, 0xa6, 0x20, 0xfc, 0x44, 0x4f,
    0x57, 0xbe, 0xbb, 0xde, 0x08, 0xcd, 0xfd, 0x8f, 0xfc, 0x82, 0xb2, 0x99, 0x1c, 0xb4, 0x51, 0xb0,
    0x2c, 0x0b, 0xd9, 0x6c, 0x16, 0xbe, 0xef, 0x23, 0x0c, 0x43, 0x10, 0x11, 0xb8, 0x99, 0x03, 0x4a,
    0x29, 0x84, 0x61, 0x88, 0x5c, 0x36, 0x83, 0x6a, 0xad, 0x81, 0x38, 0x8e, 0x91, 0xb2, 0x25, 0xce,
    0x58, 0xd6, 0x07, 0xa3, 0x0d, 0xd2, 0x99, 0x34, 0x6c, 0xcb, 0x42, 0xa9, 0xe4, 0xd1, 0x1d, 0xdf,
    0xfd, 0x11, 0x5b, 0x16, 0x7d, 0x7e, 0x78, 0x78, 0x38, 0x87, 0xa1, 0x21, 0x6e, 0x96, 0xeb, 0xd3,
    0x03, 0xc0, 0xcc, 0x04, 0x22, 0x73, 0xec, 0xd8, 0x8e, 0x3e, 0x82, 0xfe, 0xa0, 0x52, 0x31, 0xef,
    0xdc, 0xf3, 0x82, 0x7c, 0xfe, 0xc5, 0x83, 0xc8, 0xe7, 0xb3, 0x50, 0x4a, 0xc3, 0xb6, 0x6d, 0x08,
    0x21, 0x50, 0xab, 0xd5, 0x26, 0x7d, 0x10, 0x00, 0x11, 0x94, 0x32, 0x08, 0xbd, 0x10, 0xb9, 0x5c,
    0x06, 0xf5, 0x7a, 0x88, 0x20, 0x8a, 0x40, 0x20, 0xac, 0x5c, 0xd1, 0x07, 0x10, 0xe0, 0x38, 0x2e,
    0x48, 0x08, 0x74, 0xe4, 0x33, 0xe2, 0x87, 0x3f, 0xfa, 0xa9, 0x1e, 0x1d, 0x1d, 0x5f, 0xb3, 0xe1,
    0x75, 0x8b, 0x3f, 0x42, 0x9b, 0x37, 0x9b, 0x85, 0xd8, 0x27, 0x16, 0x54, 0xf3, 0x01, 0x4e, 0xc5,
    0xf8, 0x93, 0xce, 0xce, 0x5c, 0x27, 0x08, 0x7a, 0xeb, 0xb6, 0xed, 0x04, 0x48, 0x88, 0xa6, 0x7f,
    0x2c, 0xcb, 0x42, 0x10, 0x04, 0x08, 0x82, 0xa0, 0x4d, 0x9f, 0x16, 0x85, 0xb4, 0x36, 0xf0, 0x83,
    0x00, 0xd9, 0x8c, 0x8b, 0x20, 0xf0, 0xe1, 0xfb, 0x11, 0x18, 0x8c, 0xa5, 0x7d, 0xbd, 0x48, 0xd9,
    0x36, 0x88, 0x08, 0xae, 0xeb, 0x40, 0x08, 0x81, 0x6a, 0xdd, 0x13, 0x77, 0x7c, 0xe7, 0x1e, 0xb6,
    0x21, 0xff, 0xea, 0xb1, 0xc7, 0x1e, 0xec, 0x01, 0x60, 0xe6, 0x8b, 0xc2, 0x7c, 0x00, 0x08, 0xfd,
    0x03, 0x7a, 0x2f, 0xef, 0x75, 0xb4, 0x09, 0x3f, 0xa0, 0x62, 0xcd, 0x63, 0xa5, 0xb2, 0x78, 0xfa,
    0x99, 0xe7, 0x90, 0xcf, 0xe5, 0xa0, 0xb4, 0x01, 0x11, 0xc1, 0xf7, 0x7d, 0x94, 0xcb, 0xe5, 0x29,
    0xc6, 0xb7, 0xc2, 0x60, 0x8c, 0x46, 0x23, 0x0c, 0x91, 0xc9, 0x64, 0x10, 0x46, 0x11, 0xea, 0x8d,
    0x06, 0x0c, 0x03, 0xdd, 0x3d, 0x1d, 0xc8, 0x65, 0x1d, 0x68, 0x6d, 0x90, 0x4e, 0x27, 0x89, 0x9f,
    0xcf, 0x66, 0xc5, 0x4f, 0x1e, 0xd8, 0x66, 0x4a, 0x95, 0xda, 0x92, 0x0b, 0xcf, 0x5e, 0xf9, 0xa1,
    0xa6, 0xd8, 0x93, 0xa7, 0x0c, 0x60, 0x78, 0x78, 0x58, 0x12, 0x81, 0xbb, 0x0e, 0x37, 0x7e, 0x2f,
    0x9d, 0x75, 0xd7, 0x01, 0xc4, 0xbb, 0x76, 0x3e, 0x2f, 0x46, 0x27, 0x2a, 0x70, 0x5d, 0x07, 0xcc,
    0xdc, 0xf4, 0xb2, 0x86, 0x52, 0x6a, 0x1a, 0x00, 0x22, 0x82, 0x61, 0x86, 0xdf, 0x88, 0x91, 0x71,
    0x5d, 0x68, 0x1d, 0xa3, 0x5a, 0x6b, 0x00, 0xcc, 0xc8, 0x65, 0x33, 0xe8, 0xe8, 0xcc, 0x43, 0x29,
    0x85, 0x54, 0x2a, 0x05, 0x29, 0x25, 0x2c, 0x4b, 0xa2, 0x58, 0xac, 0xd0, 0xfd, 0x0f, 0x6d, 0x63,
    0xcb, 0x92, 0x7f, 0x7a, 0xef, 0xbd, 0xf7, 0x66, 0xfa, 0x07, 0x06, 0xf4, 0x5c, 0x51, 0x98, 0x13,
    0xc0, 0xd8, 0xd8, 0x18, 0x03, 0x80, 0x64, 0xdc, 0x68, 0x09, 0x62, 0x29, 0xa5, 0x79, 0x62, 0xfb,
    0x4e, 0x48, 0x69, 0x01, 0xed, 0x86, 0x95, 0x18, 0x3a, 0xdd, 0xfb, 0xed, 0x1c, 0x82, 0xef, 0x37,
    0xe0, 0x38, 0x16, 0x0c, 0x4b, 0x94, 0xcb, 0x55, 0x80, 0x18, 0xae, 0xe3, 0xa0, 0xb3, 0xb3, 0x03,
    0x5a, 0x6b, 0x58, 0x96, 0x44, 0xa2, 0xa1, 0x0c, 0x9c, 0x94, 0x23, 0x1e, 0xfe, 0xd9, 0xa3, 0xc6,
    0xb6, 0xad, 0x33, 0x5e, 0xff, 0xda, 0xbe, 0x77, 0x12, 0xc0, 0x98, 0x23, 0x0a, 0xd6, 0x5c, 0xc9,
    0x4b, 0x44, 0x7a, 0x64, 0xe4, 0x89, 0xa5, 0x88, 0xd4, 0xdb, 0xa3, 0xd0, 0x50, 0xc3, 0x53, 0xf2,
    0xd9, 0x3d, 0xfb, 0x90, 0xcd, 0xa4, 0xa1, 0xcd, 0xfc, 0xc2, 0xb1, 0x05, 0xc9, 0xf3, 0x23, 0x58,
    0x96, 0x04, 0x88, 0x50, 0xaa, 0x54, 0x41, 0x04, 0x48, 0x21, 0x91, 0x4f, 0xbb, 0x4d, 0xe1, 0x97,
    0x94, 0xd3, 0x46, 0xa3, 0x81, 0xb4, 0x9b, 0xc2, 0xce, 0x3d, 0xfb, 0x70, 0xe0, 0xe5, 0xc3, 0xbc,
    0x64, 0x49, 0xcf, 0x8d, 0x00, 0xb6, 0xa0, 0xbf, 0xdf, 0x9c, 0x7c, 0x04, 0x12, 0xd4, 0x44, 0x91,
    0x7c, 0x57, 0x3e, 0x97, 0xcb, 0x0b, 0x69, 0xe9, 0x17, 0xf6, 0x1f, 0xa0, 0xb1, 0xf1, 0x12, 0x1c,
    0xc7, 0x99, 0x24, 0x19, 0xe6, 0x2b, 0x63, 0x80, 0x17, 0x84, 0xb0, 0xa4, 0x80, 0x25, 0x80, 0x52,
    0xa9, 0x0e, 0xd3, 0x8c, 0x98, 0x9b, 0x76, 0xc0, 0x60, 0x30, 0x33, 0xec, 0x54, 0x0a, 0x44, 0x04,
    0x61, 0x09, 0xd4, 0x6b, 0xbe, 0x18, 0xde, 0xf6, 0x04, 0xa5, 0x52, 0xce, 0xc0, 0xae, 0xa7, 0x1e,
    0x39, 0x83, 0x88, 0x0c, 0xcf, 0xa2, 0x91, 0x66, 0x07, 0x90, 0xa0, 0x66, 0x32, 0x7c, 0xbd, 0x56,
    0x86, 0x2d, 0xdb, 0xc6, 0xaf, 0x77, 0xed, 0x01, 0x9b, 0x44, 0xe7, 0xb7, 0xe8, 0x21, 0x84, 0x80,
    0x94, 0x72, 0x76, 0x40, 0x44, 0xf0, 0xfd, 0x10, 0x42, 0x5a, 0xb0, 0x6c, 0x1b, 0xa5, 0x52, 0x15,
    0x6c, 0x18, 0x24, 0x80, 0x8e, 0x7c, 0xb6, 0xc9, 0x44, 0x86, 0x94, 0x32, 0x79, 0x8e, 0x61, 0xd8,
    0xb6, 0x4d, 0x8f, 0x3e, 0xfe, 0x8c, 0xb2, 0x32, 0x4e, 0xba, 0xa7, 0x2b, 0x7b, 0x4d, 0xd3, 0x9e,
    0x85, 0x03, 0x68, 0xd2, 0xc7, 0x8c, 0x8e, 0x3e, 0xb5, 0x8c, 0x88, 0xdf, 0x18, 0x84, 0x01, 0x85,
    0x61, 0x24, 0x76, 0xef, 0x7e, 0x01, 0xee, 0x64, 0xbd, 0x4f, 0x84, 0xde, 0xde, 0xde, 0x29, 0xfa,
    0xe7, 0x04, 0xe7, 0x83, 0x00, 0x78, 0x7e, 0x00, 0x61, 0x11, 0x5c, 0x37, 0x85, 0x52, 0xa5, 0x0a,
    0xa3, 0x34, 0x88, 0x08, 0x3d, 0x5d, 0x79, 0x50, 0xb2, 0x2b, 0x82, 0x10, 0x02, 0x96, 0x65, 0x25,
    0xcd, 0xcd, 0x75, 0xb0, 0xe7, 0x37, 0xfb, 0xa9, 0x32, 0x56, 0x44, 0x3a, 0xe3, 0x5e, 0x3b, 0xc9,
    0xa1, 0x0b, 0x03, 0xd0, 0x2e, 0x5d, 0x81, 0xe8, 0xef, 0xc8, 0xe7, 0x73, 0x44, 0xac, 0xc7, 0x26,
    0xc6, 0xe9, 0xd0, 0xe1, 0xb1, 0x24, 0xec, 0xcc, 0x30, 0xc6, 0x20, 0x9d, 0x4e, 0xc3, 0x75, 0x5d,
    0x48, 0x39, 0x7b, 0xa5, 0x23, 0x22, 0x78, 0xbe, 0x0f, 0x4b, 0x08, 0x64, 0x32, 0x2e, 0xca, 0xd5,
    0x3a, 0xe2, 0x58, 0x81, 0x18, 0xe8, 0xe9, 0xea, 0x84, 0x90, 0x94, 0x6c, 0xbb, 0x88, 0x12, 0x00,
    0xcc, 0x48, 0xa5, 0x6c, 0x8c, 0x4f, 0x94, 0xc5, 0xee, 0x3d, 0x7b, 0xe1, 0xa6, 0xec, 0xd7, 0xef,
    0xd8, 0xf1, 0xf3, 0x6e, 0x22, 0x9a, 0xb1, 0x27, 0x88, 0xb9, 0x6b, 0x2c, 0xf7, 0x13, 0x80, 0x94,
    0x63, 0xf3, 0xcb, 0x87, 0x8e, 0xa2, 0x5a, 0xf7, 0x60, 0x59, 0x56, 0xbb, 0x7c, 0xb6, 0x3c, 0x6f,
    0x59, 0xd6, 0xac, 0x55, 0x08, 0x04, 0x04, 0x7e, 0x04, 0x49, 0x02, 0xb9, 0x4c, 0x1a, 0xd5, 0x5a,
    0x0d, 0x41, 0x18, 0x02, 0x00, 0x7a, 0xba, 0x3b, 0x60, 0xdb, 0x89, 0xd1, 0x40, 0x12, 0x05, 0x00,
    0x90, 0x42, 0x20, 0x56, 0x11, 0x3d, 0xb3, 0x63, 0x8f, 0x71, 0xf3, 0xd9, 0xde, 0x9c, 0x45, 0x97,
    0x26, 0x0f, 0xdb, 0x22, 0x16, 0x04, 0x60, 0xa0, 0x3f, 0xa9, 0xbd, 0x0c, 0xbd, 0x21, 0x8c, 0x22,
    0x48, 0x21, 0xc4, 0xfe, 0x97, 0x0e, 0x43, 0x9b, 0xc4, 0xf0, 0x16, 0xf7, 0x6d, 0xdb, 0x86, 0x31,
    0x06, 0x42, 0x88, 0x59, 0x00, 0x24, 0x7a, 0xc8, 0x0f, 0x02, 0x80, 0x80, 0x7c, 0x36, 0x8b, 0xba,
    0xe7, 0xc3, 0x0f, 0x23, 0x30, 0x33, 0x3a, 0x3b, 0x72, 0x48, 0xbb, 0x29, 0x98, 0x13, 0x2a, 0x1a,
    0x03, 0x10, 0xc2, 0xc2, 0xee, 0xe7, 0xf6, 0x19, 0xd8, 0x16, 0x5c, 0xd7, 0xbe, 0x22, 0xf9, 0xcb,
    0xe2, 0xf9, 0x23, 0xc0, 0x9b, 0x36, 0x09, 0x10, 0xf8, 0xc8, 0x91, 0xed, 0x2b, 0x89, 0x71, 0x4e,
    0x18, 0x46, 0x80, 0x01, 0x1d, 0x78, 0xe9, 0x28, 0xa4, 0x38, 0x4e, 0x15, 0xcb, 0xb2, 0xda, 0xd4,
    0x99, 0x15, 0x00, 0x03, 0x24, 0x04, 0x82, 0x30, 0x84, 0x61, 0x8d, 0x6c, 0x2e, 0x87, 0xc0, 0x57,
    0xa8, 0xd7, 0x02, 0x00, 0x84, 0x4c, 0x2e, 0x8d, 0x6c, 0xd6, 0x85, 0x31, 0xa6, 0xed, 0x98, 0x56,
    0x71, 0x48, 0xa5, 0x1c, 0xec, 0x3f, 0x70, 0x84, 0x54, 0xc5, 0x83, 0x6b, 0xd9, 0x1b, 0x9a, 0xe4,
    0x36, 0xf3, 0x47, 0x60, 0x68, 0x88, 0x00, 0xc0, 0xd6, 0xe2, 0xbc, 0x4c, 0xc6, 0x4d, 0x2b, 0x6d,
    0x4c, 0xa8, 0x14, 0x1d, 0x7e, 0x65, 0x04, 0xb6, 0x9d, 0xd0, 0x67, 0x32, 0x6d, 0xe6, 0x2a, 0xa7,
    0xdc, 0xcc, 0x81, 0x20, 0x8c, 0x61, 0x14, 0x90, 0x4f, 0x3b, 0x88, 0xa2, 0x18, 0xd5, 0x5a, 0x0d,
    0x10, 0x31, 0xd2, 0x8e, 0x8d, 0x8e, 0x5c, 0xb6, 0xdd, 0x53, 0xb4, 0xd6, 0x6d, 0x00, 0x76, 0xca,
    0xc6, 0xb1, 0x91, 0x31, 0x71, 0xf4, 0xd8, 0x28, 0x48, 0xd2, 0x05, 0x7b, 0xf6, 0x3c, 0x9a, 0x27,
    0xda, 0x3c, 0x2d, 0x0f, 0x66, 0xa0, 0xd0, 0x56, 0x4a, 0x78, 0x88, 0xf3, 0x2c, 0xcb, 0x02, 0x8c,
    0x31, 0x75, 0x2f, 0xc2, 0xc8, 0x78, 0xb1, 0xcd, 0x7f, 0x00, 0x90, 0x52, 0xb6, 0xbd, 0xde, 0x02,
    0x35, 0x63, 0x1e, 0x11, 0x21, 0x8a, 0x14, 0xb4, 0xd6, 0xc8, 0x64, 0x1c, 0x28, 0xad, 0x50, 0xae,
    0xd5, 0x00, 0x10, 0x6c, 0xcb, 0x46, 0x3e, 0x97, 0x4b, 0xaa, 0x1a, 0x73, 0x5b, 0x8e, 0x24, 0x0e,
    0x12, 0xa8, 0x56, 0x3d, 0x3a, 0x78, 0xf8, 0x28, 0x3b, 0x8e, 0xb5, 0xc4, 0x35, 0xb4, 0xee, 0x84,
    0xfe, 0x38, 0x0b, 0x80, 0xad, 0x4d, 0x8a, 0x30, 0xaf, 0xd7, 0x71, 0x0c, 0x21, 0x04, 0x2a, 0x95,
    0x0a, 0x2a, 0xd5, 0x1a, 0x2c, 0xeb, 0xb8, 0x84, 0x10, 0x42, 0xb4, 0x93, 0x79, 0xae, 0x28, 0x10,
    0x11, 0xe2, 0x58, 0x21, 0x52, 0x31, 0x52, 0x29, 0x1b, 0xcc, 0x40, 0xb9, 0x52, 0x03, 0xd8, 0x06,
    0x09, 0x81, 0x74, 0xc6, 0x6d, 0x57, 0xb5, 0x56, 0x04, 0x12, 0xe0, 0x02, 0x7e, 0x18, 0xe0, 0x95,
    0x57, 0x8e, 0x9a, 0x74, 0x3e, 0x4b, 0x31, 0x47, 0x6b, 0x9b, 0x15, 0x52, 0xcc, 0x0d, 0xa0, 0xbf,
    0x5f, 0x03, 0x80, 0x16, 0xf2, 0x5c, 0x3f, 0x8c, 0x21, 0x05, 0x51, 0xb9, 0x54, 0x81, 0xe7, 0x05,
    0x10, 0x92, 0xc0, 0x93, 0x0c, 0x6b, 0xbd, 0xa6, 0x0e, 0xf3, 0x53, 0x49, 0x44, 0x04, 0x44, 0x91,
    0x46, 0x18, 0x6b, 0xd8, 0xb6, 0x04, 0x40, 0x28, 0x97, 0xeb, 0x00, 0x62, 0x08, 0x00, 0xb6, 0xb4,
    0x01, 0x00, 0x4a, 0xab, 0x29, 0xb9, 0xd0, 0x92, 0xe2, 0x47, 0x8e, 0x8d, 0x30, 0x2c, 0x09, 0x20,
    0x5e, 0x0b, 0x00, 0xfd, 0x73, 0xe5, 0x40, 0xb3, 0x81, 0xf1, 0xae, 0x5d, 0xbb, 0x52, 0x44, 0xbc,
    0x2a, 0x0c, 0x03, 0x08, 0x29, 0x68, 0xbc, 0x54, 0x46, 0x1c, 0x47, 0x53, 0x8c, 0x9e, 0x6c, 0xb0,
    0x52, 0x6a, 0xe6, 0x46, 0xc6, 0x09, 0x50, 0xa5, 0x63, 0x44, 0x51, 0x04, 0x21, 0x04, 0x04, 0x08,
    0xa5, 0x4a, 0x0d, 0xda, 0x98, 0x44, 0x13, 0x49, 0x02, 0x21, 0x89, 0x92, 0x99, 0xa6, 0xaf, 0x08,
    0xc7, 0x46, 0x27, 0x00, 0xa3, 0x01, 0x25, 0xd6, 0x4e, 0x22, 0xc8, 0xdc, 0x65, 0x74, 0x71, 0x16,
    0x3d, 0x60, 0xbd, 0x28, 0x8a, 0x02, 0x08, 0x41, 0x54, 0x2c, 0x55, 0xa0, 0x94, 0x06, 0x26, 0xd9,
    0x38, 0x39, 0xdc, 0x71, 0x1c, 0xcf, 0x49, 0x21, 0xa5, 0x22, 0x44, 0x61, 0x0c, 0x21, 0x05, 0x48,
    0x10, 0x6a, 0xd5, 0x3a, 0x54, 0x9c, 0x6c, 0x2d, 0xa4, 0x14, 0xd3, 0x9e, 0x71, 0x3c, 0x0a, 0x40,
    0xb1, 0x58, 0x25, 0x8e, 0x42, 0x18, 0xa8, 0xd5, 0x09, 0x41, 0xa6, 0x76, 0x64, 0x31, 0x93, 0x80,
    0x34, 0x52, 0x2d, 0x02, 0x89, 0x5c, 0x1c, 0x33, 0xc0, 0x84, 0x72, 0xa9, 0x0e, 0x66, 0x80, 0xcd,
    0x54, 0xaf, 0xb7, 0xbe, 0x28, 0x8a, 0xa2, 0x59, 0x1b, 0x59, 0x12, 0x01, 0x83, 0x28, 0x52, 0x90,
    0x42, 0x42, 0x48, 0x81, 0x5a, 0xdd, 0x43, 0x14, 0x47, 0x10, 0x02, 0xb0, 0xa4, 0x80, 0x99, 0xe5,
    0x19, 0x42, 0x10, 0xca, 0xe5, 0x2a, 0x45, 0x7e, 0x08, 0x01, 0xea, 0x6b, 0x3e, 0x6f, 0x2e, 0x00,
    0x5b, 0x92, 0x0a, 0x04, 0xd3, 0xeb, 0x3a, 0x0e, 0x19, 0x63, 0x8c, 0x36, 0x9a, 0xca, 0x95, 0x1a,
    0x98, 0x19, 0xda, 0x24, 0x46, 0x0b, 0x21, 0x10, 0x86, 0x21, 0x82, 0x20, 0x80, 0xe7, 0x79, 0x33,
    0x0e, 0x33, 0x93, 0x7d, 0x62, 0x34, 0x10, 0x04, 0x21, 0x84, 0x20, 0x48, 0x41, 0x68, 0x78, 0x21,
    0x82, 0x20, 0x04, 0x09, 0x01, 0x69, 0x59, 0x50, 0x71, 0x0c, 0x15, 0xc7, 0x53, 0x9f, 0xc1, 0x0c,
    0x22, 0x01, 0xcf, 0x6b, 0xc0, 0xf7, 0x03, 0xb0, 0xe1, 0xae, 0xe1, 0xe1, 0x61, 0x77, 0xd2, 0x6e,
    0x76, 0xfa, 0x3c, 0xb0, 0x75, 0xeb, 0xe2, 0x66, 0x09, 0x35, 0x4b, 0x52, 0xb6, 0x0d, 0x30, 0xb3,
    0xd6, 0x01, 0x6a, 0xf5, 0x7a, 0x93, 0x0a, 0x1a, 0x8e, 0x73, 0xfc, 0xfd, 0x13, 0x13, 0x13, 0xd3,
    0x12, 0x7a, 0x06, 0x31, 0x9a, 0x0c, 0x35, 0x51, 0x04, 0x12, 0xb2, 0xd9, 0x99, 0x43, 0xd4, 0x3d,
    0x1f, 0xb2, 0xa9, 0x64, 0xc3, 0x30, 0x82, 0xe1, 0x13, 0xbb, 0x71, 0x73, 0xb7, 0x1a, 0xc5, 0xe4,
    0x7b, 0x01, 0xc0, 0x9c, 0xcf, 0x64, 0x54, 0x16, 0x40, 0x30, 0x6b, 0x04, 0xfa, 0xdb, 0xf4, 0xe0,
    0xbe, 0x66, 0x77, 0xe5, 0x28, 0x04, 0x1a, 0x0d, 0x2f, 0x59, 0x91, 0xc4, 0x0a, 0x53, 0x12, 0x61,
    0xc1, 0x5b, 0x3d, 0x83, 0x20, 0x0c, 0x21, 0xa5, 0x80, 0x20, 0x81, 0x28, 0x8c, 0x50, 0x6f, 0xf8,
    0x10, 0x52, 0x40, 0x08, 0x01, 0x3f, 0xf0, 0xa7, 0xed, 0xb3, 0x92, 0x02, 0x00, 0xc4, 0x91, 0x81,
    0xef, 0x87, 0x60, 0x03, 0x27, 0x95, 0x22, 0x37, 0xe9, 0xb5, 0x43, 0x34, 0x33, 0x85, 0xfa, 0x5b,
    0x41, 0xe7, 0xde, 0xc4, 0x7b, 0x8c, 0x38, 0x8e, 0x50, 0xf7, 0x7c, 0x08, 0x41, 0x88, 0xe3, 0x08,
    0xe6, 0xe4, 0xed, 0x07, 0xf3, 0x71, 0x0a, 0x41, 0x10, 0x62, 0xa5, 0x50, 0xab, 0x37, 0x00, 0x29,
    0x60, 0x8c, 0x41, 0x14, 0x86, 0x20, 0x41, 0xd3, 0x46, 0xd1, 0x56, 0xb1, 0x88, 0x63, 0x05, 0x08,
    0x23, 0x53, 0x86, 0xac, 0xb9, 0xa5, 0x44, 0xb3, 0x46, 0x49, 0x21, 0xbb, 0x01, 0x02, 0x09, 0x42,
    0x18, 0xf9, 0xf0, 0x7c, 0x1f, 0xc2, 0x92, 0x88, 0xe3, 0x18, 0x7a, 0x4e, 0xbe, 0xcf, 0xfe, 0x0a,
    0x7d, 0x95, 0x00, 0x20, 0x86, 0xd6, 0x06, 0x95, 0x4a, 0x1d, 0x90, 0x4d, 0xa7, 0x18, 0x33, 0x6d,
    0x9f, 0xc8, 0xcd, 0x73, 0x58, 0x66, 0x06, 0x1b, 0x0d, 0xc0, 0x50, 0x1c, 0x2b, 0x01, 0x00, 0x43,
    0xb3, 0x01, 0x68, 0xd5, 0x58, 0xc3, 0x9c, 0x67, 0xc3, 0x20, 0x92, 0x88, 0x22, 0x86, 0xef, 0x45,
    0x90, 0x24, 0xa1, 0xb4, 0x6e, 0x6f, 0xde, 0x16, 0x7e, 0xf8, 0x0a, 0x30, 0x0c, 0xc2, 0x30, 0x84,
    0xa4, 0xa4, 0xe6, 0x33, 0x33, 0xaa, 0xf5, 0x06, 0x00, 0x01, 0x21, 0x66, 0x0f, 0x1b, 0x37, 0x59,
    0xc0, 0x24, 0x00, 0xb6, 0xe1, 0x38, 0x29, 0x9e, 0x13, 0x40, 0x7f, 0xff, 0x58, 0x73, 0xd4, 0xe2,
    0x1c, 0x83, 0x21, 0x85, 0x80, 0xd6, 0x1a, 0xc1, 0x24, 0xa3, 0x7d, 0xdf, 0x5f, 0xf8, 0x3c, 0x0c,
    0x00, 0xcd, 0x82, 0xd1, 0x7a, 0x06, 0x25, 0x7e, 0x4d, 0xd6, 0x2b, 0x84, 0x76, 0x1f, 0x38, 0x31,
    0x07, 0x0c, 0x9b, 0x66, 0x1e, 0xb4, 0x40, 0xb2, 0x51, 0xb1, 0xd0, 0x0b, 0x9a, 0x07, 0x04, 0x53,
    0x0a, 0x60, 0x10, 0x04, 0xb1, 0x49, 0xbc, 0x0e, 0x42, 0xbb, 0x7c, 0xc6, 0x27, 0x96, 0xbc, 0x39,
    0x03, 0x90, 0x94, 0xc3, 0x70, 0xf2, 0x67, 0x88, 0xe0, 0x79, 0x01, 0x60, 0x4c, 0xa2, 0xaf, 0x66,
    0x28, 0x0c, 0x49, 0x57, 0x66, 0xd8, 0xb6, 0x84, 0x25, 0x05, 0x18, 0x26, 0x66, 0x37, 0x8e, 0x9a,
    0x49, 0xcc, 0x73, 0x02, 0x50, 0x26, 0xa6, 0x44, 0xcb, 0x27, 0x09, 0xa8, 0x94, 0x6e, 0x73, 0xd2,
    0x18, 0x83, 0x7a, 0xb3, 0xac, 0x9e, 0x04, 0x8b, 0x92, 0xdc, 0x81, 0x6c, 0x1f, 0x3d, 0x95, 0x2b,
    0x55, 0xc0, 0x10, 0x6c, 0x69, 0x35, 0xdf, 0x31, 0x15, 0x84, 0xd1, 0x06, 0x6c, 0x92, 0xb9, 0xc0,
    0x4e, 0xd9, 0x20, 0x36, 0x9e, 0xe7, 0x45, 0xfe, 0x82, 0x22, 0x50, 0x6b, 0x34, 0x14, 0x90, 0x0c,
    0x23, 0x49, 0xb7, 0x35, 0x30, 0x5a, 0xb7, 0xa3, 0xd0, 0x68, 0x34, 0xa6, 0xed, 0x41, 0x67, 0x9f,
    0x09, 0x08, 0x0c, 0x82, 0xd6, 0x89, 0x34, 0x10, 0x42, 0xa2, 0xe1, 0x79, 0x28, 0x96, 0x4b, 0x4d,
    0x59, 0x2e, 0x00, 0x9a, 0x5e, 0x81, 0xb4, 0x49, 0x28, 0x94, 0x71, 0x5d, 0x76, 0x6c, 0x1b, 0x1a,
    0xa8, 0x1c, 0xdc, 0x70, 0x75, 0xe3, 0x44, 0xba, 0xcd, 0x08, 0x20, 0xf0, 0x43, 0x2f, 0x8e, 0x63,
    0x48, 0x61, 0xb1, 0xb4, 0xec, 0x64, 0x46, 0x8d, 0xd5, 0x94, 0x0f, 0x96, 0xcb, 0xe5, 0x05, 0xe6,
    0x02, 0x83, 0x89, 0xa0, 0x4c, 0x62, 0x6c, 0xac, 0x22, 0x54, 0x2a, 0x15, 0x68, 0xa5, 0x81, 0xf6,
    0x60, 0xc4, 0xd3, 0xe8, 0xc3, 0xcc, 0x30, 0x6c, 0x90, 0xcb, 0xb9, 0x6c, 0xa7, 0x6c, 0xb0, 0xe1,
    0x89, 0xc1, 0xe4, 0x20, 0x30, 0xd9, 0xde, 0xcf, 0x58, 0x85, 0x9a, 0x9d, 0x98, 0x35, 0x4a, 0x41,
    0x10, 0x34, 0xe7, 0x5e, 0x0b, 0xe9, 0xb4, 0x83, 0x28, 0x8e, 0xda, 0xe6, 0x13, 0x11, 0xa2, 0x28,
    0x42, 0xa9, 0x54, 0x6a, 0x0f, 0xe2, 0xf3, 0x66, 0x02, 0x27, 0x46, 0x15, 0x27, 0x26, 0x12, 0xed,
    0x6f, 0x92, 0x2f, 0xb2, 0xa4, 0x9c, 0x42, 0xa0, 0xd6, 0xae, 0xb5, 0x35, 0x24, 0x75, 0x76, 0x76,
    0xb2, 0x65, 0xdb, 0x30, 0x10, 0x47, 0x12, 0xb5, 0x33, 0x75, 0xb0, 0x9f, 0x71, 0xb5, 0x28, 0xa5,
    0x7c, 0x25, 0x08, 0x02, 0xb0, 0x21, 0xb8, 0x4e, 0x0a, 0x1d, 0xf9, 0x1c, 0xa2, 0x28, 0x3e, 0x41,
    0x68, 0x25, 0x54, 0x12, 0x42, 0xa0, 0xab, 0xab, 0x6b, 0x9a, 0xc4, 0x9e, 0x3e, 0x17, 0x08, 0x84,
    0x51, 0x84, 0x28, 0x8a, 0xdb, 0xfb, 0x1f, 0x30, 0xc3, 0x92, 0x22, 0xf1, 0xe3, 0xa4, 0x8f, 0x26,
    0xca, 0x34, 0xf9, 0xc5, 0x92, 0xde, 0x6e, 0x90, 0x20, 0x28, 0xd6, 0x7b, 0x01, 0x60, 0xeb, 0xe2,
    0xc5, 0x34, 0x6f, 0x0e, 0x90, 0x90, 0x2f, 0xc6, 0xb1, 0x82, 0xe7, 0xf9, 0x94, 0x72, 0x52, 0x58,
    0xd4, 0xdb, 0x05, 0xa5, 0x9a, 0x7a, 0x9d, 0xa6, 0x82, 0xa8, 0xd5, 0x6a, 0x18, 0x1f, 0x1f, 0x87,
    0xd6, 0x7a, 0xc6, 0x68, 0xb4, 0x4e, 0x6b, 0x7c, 0xdf, 0x83, 0x20, 0x31, 0x65, 0x0c, 0x35, 0xad,
    0x55, 0x0a, 0x4d, 0xa5, 0x4f, 0x4b, 0x5a, 0x4b, 0x21, 0xb1, 0x6c, 0x79, 0x2f, 0xe9, 0xd8, 0x00,
    0x1a, 0xbb, 0xe7, 0x5d, 0x6c, 0xb5, 0xb4, 0xb6, 0x02, 0x3f, 0xe7, 0x79, 0xa1, 0x8e, 0xe2, 0x50,
    0x32, 0x1b, 0x5e, 0xb1, 0x6c, 0x09, 0x94, 0xd6, 0x50, 0x5a, 0x37, 0xeb, 0xf8, 0x54, 0x10, 0xbe,
    0xef, 0x63, 0x74, 0x74, 0xb4, 0x5d, 0x9d, 0x5a, 0x40, 0x84, 0x10, 0x60, 0x63, 0x30, 0x3e, 0x3e,
    0xde, 0x8c, 0xd6, 0xa4, 0xc3, 0x0f, 0x4e, 0x4a, 0xec, 0x64, 0xd0, 0xc9, 0xf8, 0x19, 0x41, 0x6b,
    0x03, 0xc3, 0x0c, 0xd7, 0xb5, 0x78, 0xd5, 0xf2, 0xa5, 0xb2, 0x5a, 0xa9, 0xeb, 0xd8, 0x44, 0x3b,
    0xe6, 0x9f, 0x07, 0x9a, 0xd9, 0x14, 0x72, 0xcf, 0x8b, 0xcc, 0xea, 0x25, 0x27, 0x65, 0x41, 0x6b,
    0xc3, 0xab, 0x57, 0xad, 0x80, 0x25, 0x04, 0xa2, 0x28, 0x9c, 0x79, 0x70, 0x6f, 0x36, 0xbc, 0x62,
    0xb1, 0x88, 0xf1, 0xf1, 0x71, 0x04, 0x41, 0x00, 0x63, 0x0c, 0x1a, 0x8d, 0x06, 0x46, 0x47, 0x47,
    0xe1, 0xfb, 0x89, 0xf2, 0xc4, 0x09, 0x5c, 0xc7, 0x09, 0xee, 0x60, 0xe6, 0x44, 0x66, 0x13, 0xa0,
    0x62, 0x85, 0x9e, 0x9e, 0x6e, 0x5e, 0xbd, 0x6a, 0x25, 0x82, 0x58, 0x1d, 0x78, 0xf9, 0x28, 0xbf,
    0x70, 0x1c, 0xfa, 0x2c, 0x00, 0x12, 0xe5, 0x5b, 0x90, 0x97, 0x5c, 0x72, 0x49, 0x0c, 0xe2, 0x5f,
    0xba, 0xae, 0xc3, 0x71, 0x1c, 0x9b, 0x35, 0x67, 0xf4, 0x21, 0x9f, 0xcf, 0x20, 0x08, 0xc2, 0x19,
    0xc6, 0xbe, 0xe3, 0x06, 0x09, 0x21, 0x10, 0x04, 0x01, 0xc6, 0xc6, 0xc6, 0x30, 0x32, 0x32, 0x82,
    0x62, 0xb1, 0x08, 0xa5, 0xd4, 0x14, 0xea, 0xb4, 0x9b, 0xb3, 0xc0, 0x34, 0x40, 0x71, 0x1c, 0x43,
    0x29, 0x05, 0x29, 0x04, 0xa2, 0x48, 0xe1, 0xcc, 0x55, 0x2b, 0xcc, 0xb2, 0x25, 0x3d, 0xac, 0x62,
    0xbd, 0x6d, 0x70, 0x70, 0x30, 0x4a, 0x2e, 0x48, 0x81, 0xe7, 0xcc, 0x81, 0x56, 0x25, 0x02, 0xec,
    0x07, 0x89, 0x99, 0xa2, 0x38, 0xa6, 0x65, 0xcb, 0x16, 0x61, 0xe5, 0xb2, 0x25, 0x08, 0xc2, 0x08,
    0x4a, 0xc5, 0xf3, 0xe8, 0x7f, 0x9a, 0x32, 0x98, 0xb7, 0xde, 0x3b, 0x25, 0x3f, 0x38, 0x99, 0xb6,
    0x88, 0x44, 0x7b, 0x27, 0xc4, 0xcc, 0xf0, 0x3c, 0xaf, 0xdd, 0xa9, 0x95, 0xd1, 0xb8, 0x60, 0xfd,
    0x3a, 0x22, 0x62, 0xf2, 0x43, 0x75, 0x6f, 0xf2, 0x87, 0x8d, 0xf3, 0x2f, 0x77, 0xfb, 0x9b, 0x5b,
    0x09, 0x5f, 0x98, 0x47, 0x2a, 0x75, 0xbf, 0x2c, 0x48, 0xc8, 0x7c, 0x26, 0xcb, 0x17, 0x9c, 0x7f,
    0x36, 0xa2, 0x38, 0x19, 0xce, 0x4f, 0x4a, 0x0b, 0x11, 0x00, 0x36, 0x90, 0x82, 0xda, 0x3b, 0x50,
    0x06, 0xb7, 0x0f, 0x3c, 0x94, 0x52, 0x20, 0x00, 0x8d, 0x46, 0x03, 0x5a, 0x27, 0x5b, 0x6b, 0x63,
    0x0c, 0xd2, 0x4e, 0x8a, 0x37, 0x5c, 0x7c, 0x81, 0x2c, 0x55, 0xea, 0xa3, 0xa5, 0x63, 0xc1, 0x70,
    0x2b, 0xc7, 0xe7, 0x05, 0x40, 0x44, 0x5c, 0x28, 0x14, 0xe4, 0x45, 0x17, 0x5d, 0x33, 0xc6, 0x10,
    0xff, 0x9e, 0xcd, 0xb8, 0x30, 0x0c, 0xbd, 0xe1, 0xe2, 0x0b, 0x60, 0x59, 0x22, 0x01, 0x11, 0x46,
    0x27, 0x25, 0x25, 0x04, 0x33, 0xa4, 0x25, 0x93, 0x6e, 0xce, 0x89, 0xd2, 0xb4, 0x6c, 0x1b, 0x68,
    0xd2, 0xa6, 0xde, 0x68, 0xb4, 0x55, 0x6e, 0x72, 0xbf, 0x22, 0xc2, 0xaa, 0x15, 0x7d, 0xfa, 0x75,
    0xeb, 0xcf, 0x86, 0x1f, 0xa9, 0xbb, 0xaf, 0x1c, 0x1c, 0xac, 0x24, 0xf4, 0x21, 0x3e, 0xa9, 0xed,
    0x34, 0x1b, 0x71, 0x5b, 0x1c, 0x2b, 0x78, 0x7e, 0x28, 0x7e, 0x77, 0xfd, 0xb9, 0x58, 0xb5, 0xbc,
    0x0f, 0x71, 0xac, 0xe0, 0x07, 0xfe, 0x94, 0xad, 0xc4, 0x42, 0xe4, 0x84, 0x65, 0x49, 0x28, 0xad,
    0x9a, 0x0d, 0x0a, 0x70, 0xed, 0x14, 0x20, 0x2d, 0x94, 0x2b, 0x75, 0x78, 0x9e, 0xdf, 0xa6, 0x18,
    0x09, 0x81, 0x30, 0x8c, 0xf8, 0xcd, 0x97, 0x5f, 0x24, 0x48, 0x4a, 0x5d, 0x2a, 0x4e, 0xfc, 0x4b,
    0x93, 0x3e, 0xbc, 0xe0, 0xf3, 0x81, 0xc1, 0xc1, 0x41, 0xbd, 0x69, 0xd3, 0x26, 0x71, 0xfe, 0x65,
    0x57, 0xfd, 0xdc, 0x0f, 0x83, 0xc7, 0x9c, 0x94, 0x2d, 0x16, 0xf5, 0x74, 0xea, 0xb7, 0xbc, 0xe1,
    0x62, 0x04, 0x41, 0x04, 0x80, 0x51, 0xaf, 0xd7, 0xe7, 0x9d, 0x87, 0x27, 0xef, 0x48, 0x2d, 0x3b,
    0x85, 0x28, 0xd2, 0xed, 0x5d, 0x51, 0x3a, 0x9d, 0x02, 0x04, 0x50, 0xab, 0xd7, 0xa6, 0x08, 0x39,
    0xad, 0x35, 0xd2, 0xe9, 0x94, 0xb9, 0xee, 0x9a, 0x01, 0x11, 0x04, 0xd1, 0x7d, 0x97, 0x5f, 0xf9,
    0x47, 0x3b, 0x0b, 0x85, 0x82, 0x9c, 0xed, 0x36, 0xd7, 0xac, 0x11, 0x18, 0x1a, 0x5a, 0x9f, 0x5c,
    0x2f, 0x20, 0xe7, 0x73, 0xc9, 0xa5, 0x8d, 0x08, 0xef, 0x78, 0xdb, 0x1b, 0x90, 0xcf, 0xa6, 0xc1,
    0x26, 0x59, 0xab, 0xd4, 0x6a, 0xb5, 0xf6, 0x36, 0x6d, 0x9e, 0xd3, 0x7e, 0xa4, 0x6c, 0x0b, 0x61,
    0x14, 0x81, 0x04, 0x10, 0x85, 0x11, 0xa4, 0x20, 0x40, 0x08, 0x94, 0x2b, 0xb5, 0x76, 0x31, 0x95,
    0x42, 0xa0, 0xd1, 0xf0, 0xf9, 0xf2, 0x4b, 0x5f, 0x8b, 0x35, 0xab, 0x96, 0xe9, 0xf1, 0x62, 0xe5,
    0x73, 0xf3, 0xb9, 0x47, 0xcc, 0x5e, 0x4d, 0x06, 0x35, 0x33, 0x8b, 0xf3, 0x2e, 0xb9, 0xea, 0xa1,
    0x86, 0xe7, 0x3d, 0x68, 0x59, 0x52, 0xbe, 0x66, 0xdd, 0x5a, 0xfd, 0x96, 0x37, 0x5e, 0x88, 0x5a,
    0xa3, 0x01, 0xcb, 0xb2, 0x10, 0xc7, 0x31, 0xaa, 0xd5, 0x6a, 0x7b, 0xa7, 0x33, 0xf3, 0x4f, 0xb2,
    0xcc, 0x72, 0x9d, 0x14, 0x1a, 0x5e, 0x03, 0xf5, 0x7a, 0x1d, 0xd5, 0x6a, 0x15, 0x8e, 0xeb, 0x02,
    0xc6, 0x60, 0x62, 0xbc, 0x74, 0xbc, 0x3b, 0x27, 0xda, 0x55, 0x7f, 0xf0, 0xfd, 0xd7, 0x4b, 0x6d,
    0xcc, 0xb7, 0x2e, 0xb9, 0xe2, 0xfa, 0x5f, 0x7d, 0xbf, 0x50, 0x90, 0x83, 0x83, 0xb3, 0xdf, 0x2d,
    0x5d, 0xd0, 0x5d, 0x09, 0x81, 0xcc, 0xc7, 0xfc, 0x50, 0x7b, 0x10, 0xc0, 0x0d, 0xbf, 0x7f, 0x0d,
    0x67, 0x33, 0x2e, 0xb4, 0x36, 0x10, 0x22, 0x11, 0x5e, 0xb5, 0x5a, 0x0d, 0xd5, 0x6a, 0xb5, 0x7d,
    0xd9, 0x23, 0x8a, 0x22, 0xc4, 0x71, 0x8c, 0x38, 0x8e, 0x11, 0x85, 0x21, 0xbc, 0x7a, 0x03, 0x04,
    0xa0, 0x54, 0x2c, 0xc3, 0xf7, 0x7d, 0x10, 0x01, 0x1d, 0x1d, 0x79, 0x20, 0xd6, 0x18, 0x1b, 0x2f,
    0x42, 0x4a, 0x01, 0x29, 0x04, 0xca, 0x95, 0xaa, 0xb9, 0xfa, 0xca, 0x37, 0xca, 0xd7, 0x9c, 0xb3,
    0xe6, 0x48, 0xb5, 0xaa, 0x3f, 0xcd, 0xbc, 0x49, 0x6c, 0xdc, 0xb8, 0xd1, 0x9c, 0xf2, 0x49, 0x3d,
    0x11, 0x19, 0x6c, 0xd9, 0x22, 0xce, 0xbb, 0xec, 0x6d, 0xcf, 0xc7, 0x11, 0x7f, 0x82, 0x04, 0xc9,
    0xdf, 0x39, 0x7f, 0x9d, 0xba, 0xee, 0x9d, 0xfd, 0xa8, 0xd6, 0x6a, 0xed, 0x15, 0x7b, 0xab, 0x09,
    0x79, 0x9e, 0x87, 0x7a, 0xbd, 0x8e, 0x5a, 0xad, 0xd6, 0x04, 0x55, 0x43, 0xad, 0x5e, 0x83, 0xef,
    0xfb, 0xb0, 0xa4, 0x40, 0xbd, 0xee, 0x43, 0x08, 0x09, 0x22, 0x81, 0xee, 0xae, 0x3c, 0xfc, 0xba,
    0x8f, 0x91, 0xb1, 0x12, 0x6c, 0xdb, 0x46, 0x18, 0x45, 0xdc, 0xd5, 0x99, 0x37, 0x1f, 0xba, 0xe9,
    0xbd, 0xe4, 0xa6, 0x9d, 0x1b, 0xcf, 0xdf, 0x70, 0xe5, 0xc4, 0x96, 0x2d, 0x09, 0x8d, 0x4f, 0xeb,
    0xb2, 0x07, 0x0d, 0x0e, 0xea, 0xe1, 0xe1, 0x61, 0x6b, 0xfd, 0x86, 0x77, 0xdc, 0x5a, 0xac, 0xd4,
    0xee, 0xb4, 0x52, 0x29, 0xfb, 0xc6, 0x1b, 0xae, 0x8b, 0xd7, 0x9d, 0xb9, 0x02, 0x0d, 0x2f, 0x38,
    0x5e, 0x3d, 0x4e, 0xa0, 0x4e, 0x6b, 0xa9, 0x45, 0x44, 0x90, 0x52, 0x20, 0xe5, 0xd8, 0x28, 0x96,
    0xab, 0x20, 0x00, 0x24, 0x08, 0x8b, 0x7b, 0xba, 0x30, 0x36, 0x51, 0xc2, 0xf8, 0x44, 0x19, 0x76,
    0xca, 0xe2, 0x4a, 0xb5, 0xa6, 0x3e, 0xfb, 0x97, 0x37, 0x5b, 0xe7, 0xae, 0x5d, 0xfb, 0x99, 0x45,
    0xab, 0x2f, 0xff, 0xe9, 0xf0, 0xf0, 0xb0, 0x35, 0x17, 0x75, 0x4e, 0xea, 0xbe, 0x50, 0x7f, 0x7f,
    0xbf, 0x66, 0x66, 0xe1, 0x76, 0x9f, 0x7b, 0xf3, 0xd8, 0x58, 0xe9, 0x3f, 0x57, 0x9f, 0xb1, 0xd4,
    0xfe, 0xf8, 0x47, 0xdf, 0x1f, 0x13, 0x0c, 0x0c, 0xcf, 0x9d, 0xc4, 0xdc, 0xec, 0xc2, 0x52, 0x5a,
    0x18, 0x6f, 0xce, 0x0f, 0x96, 0x94, 0x58, 0xd4, 0xdb, 0x8d, 0x97, 0x5e, 0x3e, 0x84, 0x5a, 0xdd,
    0x33, 0xd5, 0x4a, 0x8d, 0x3f, 0xf0, 0xc7, 0xd7, 0xdb, 0x57, 0xbf, 0xe3, 0xcd, 0x5f, 0xec, 0x59,
    0x7d, 0xd9, 0x17, 0x86, 0x87, 0x87, 0xad, 0x81, 0x81, 0x01, 0xf5, 0xaa, 0x5d, 0x78, 0x22, 0x22,
    0x1e, 0x1a, 0x1a, 0xc2, 0x39, 0xe7, 0x9c, 0x13, 0x46, 0x5e, 0xf6, 0xba, 0x57, 0x8e, 0x16, 0x7f,
    0x71, 0xf5, 0xdb, 0xdf, 0x64, 0x7f, 0xf4, 0xc3, 0x37, 0xa8, 0x72, 0xa9, 0x6a, 0x92, 0x51, 0x91,
    0x66, 0x5d, 0x8f, 0x58, 0x96, 0x04, 0x03, 0x28, 0x16, 0xcb, 0x20, 0x01, 0xa4, 0x6c, 0x0b, 0x3d,
    0x3d, 0x3d, 0xbc, 0x63, 0xf7, 0x6f, 0x54, 0xa9, 0x52, 0x11, 0xd7, 0x5d, 0xfb, 0x36, 0xf1, 0xe1,
    0x0f, 0xfc, 0xc1, 0xa7, 0x96, 0xae, 0x7d, 0xd3, 0xa7, 0x0a, 0x85, 0x82, 0x5c, 0xa8, 0xf1, 0x27,
    0x75, 0x67, 0x6e, 0xf3, 0xe6, 0xcd, 0x86, 0x79, 0x93, 0x78, 0xdd, 0xc0, 0x40, 0x39, 0x4a, 0xc9,
    0xb7, 0x1f, 0x3c, 0x74, 0xf4, 0x7b, 0x1f, 0xbe, 0xf1, 0xbd, 0xd6, 0x27, 0xff, 0xe2, 0x46, 0x51,
    0x2c, 0x55, 0x4c, 0xc3, 0x0b, 0xb5, 0x10, 0x64, 0xa6, 0x6d, 0xd8, 0x4c, 0x72, 0xde, 0x15, 0xc5,
    0x31, 0x8a, 0xc5, 0x2a, 0x04, 0x49, 0x93, 0xce, 0xb8, 0xca, 0x71, 0x6c, 0xda, 0xfe, 0xf4, 0x2e,
    0xeb, 0xca, 0xb7, 0xbe, 0xe1, 0xd0, 0xa6, 0x4f, 0x7f, 0xec, 0xda, 0x75, 0x17, 0xbc, 0xf5, 0x8b,
    0x85, 0xc2, 0x46, 0x39, 0x38, 0x38, 0x78, 0x52, 0xb7, 0x77, 0x4f, 0xea, 0xea, 0x71, 0xeb, 0x90,
    0x8d, 0x88, 0x1a, 0x00, 0xde, 0xb7, 0x6b, 0xfb, 0xfd, 0xf7, 0xdc, 0xf4, 0xbe, 0xf7, 0xfc, 0xf5,
    0x8a, 0xe5, 0x7d, 0x97, 0xde, 0x79, 0xd7, 0xbd, 0xd8, 0xfd, 0xfc, 0x7e, 0x80, 0x01, 0xdb, 0xb6,
    0xb5, 0x10, 0x4d, 0xc5, 0x9f, 0x2c, 0x35, 0x51, 0xae, 0x54, 0xa8, 0x52, 0xad, 0x49, 0x80, 0xc5,
    0x8a, 0x65, 0x7d, 0xc2, 0xf7, 0xbc, 0x46, 0x77, 0x57, 0xd7, 0xb7, 0xbf, 0xfd, 0x9d, 0x1f, 0x6c,
    0x26, 0xa2, 0x91, 0xc4, 0xf8, 0x2d, 0x27, 0x7d, 0x15, 0xff, 0x94, 0xae, 0xfb, 0x26, 0xeb, 0xed,
    0x21, 0x22, 0xda, 0x6c, 0x00, 0x10, 0xfb, 0xfb, 0xaf, 0xda, 0xb9, 0xf3, 0xf9, 0x1b, 0xee, 0xbb,
    0xef, 0x67, 0x03, 0x8f, 0x3d, 0xf9, 0xab, 0x33, 0x0e, 0x1d, 0x1d, 0x49, 0x6e, 0x66, 0x31, 0x43,
    0x29, 0x8d, 0x9e, 0xee, 0x0e, 0xbc, 0xfb, 0xda, 0x01, 0xdc, 0x7e, 0xc7, 0x0f, 0x60, 0xdb, 0xd6,
    0xee, 0x73, 0xd6, 0xad, 0xf9, 0xe1, 0x03, 0x85, 0x3b, 0xfe, 0x8d, 0xba, 0x56, 0xef, 0x03, 0x92,
    0xff, 0xe8, 0x58, 0x48, 0xc2, 0x9e, 0x76, 0x04, 0x26, 0xe7, 0x04, 0x00, 0xe6, 0x42, 0x41, 0xd2,
    0xe0, 0xa0, 0xa6, 0xf4, 0x59, 0x0f, 0x01, 0x78, 0x88, 0x99, 0x33, 0xe5, 0xb1, 0xe7, 0xce, 0xbd,
    0xed, 0xf6, 0xef, 0xbf, 0x66, 0xdb, 0xb6, 0xc7, 0xfb, 0x0e, 0x1c, 0x3a, 0x92, 0x31, 0x26, 0x72,
    0xa4, 0xb0, 0x82, 0x72, 0xc5, 0x3b, 0x62, 0x8c, 0x7e, 0x76, 0xdf, 0xbe, 0x97, 0x77, 0x11, 0x91,
    0xa6, 0xae, 0xd5, 0xd8, 0xb8, 0x71, 0xa3, 0x2c, 0x14, 0x0a, 0x86, 0x88, 0x4e, 0xeb, 0x9f, 0x20,
    0x4e, 0xf7, 0x45, 0xcc, 0x05, 0x59, 0x38, 0x89, 0xcb, 0xaa, 0x57, 0x5c, 0x71, 0x85, 0x95, 0xdc,
    0x7e, 0x3f, 0xfd, 0xd7, 0x7f, 0x01, 0x5a, 0xaf, 0x97, 0x1e, 0xda, 0x9a, 0x8a, 0x0d, 0x00, 0x00,
    0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82,
};
static const int semechkiPngDataSize = sizeof (semechkiPngData);
}

NewProjectAudioProcessorEditor::
NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)

    : AudioProcessorEditor (&p),
      audioProcessor (p)
{
    setWantsKeyboardFocus (true);
    setMouseClickGrabsKeyboardFocus (true);
    juce::Random random;

    for (int i = 0; i < 45; ++i)
    {
        starX[i] = random.nextFloat();
        starY[i] = random.nextFloat();

        starSpeed[i] = 0.002f +
            random.nextFloat() * 0.008f;

        starSize[i] = 1.0f +
            random.nextFloat() * 3.0f;
    }

    loadFaceImages();

    semechkiImage = juce::ImageFileFormat::loadFrom (
        semechkiPngData,
        semechkiPngDataSize);

    setupAdvancedControls();

    setupSlider (
        pitchDropSlider,
        pitchDropLabel,
        "CYKA");

    pitchDropSlider.setRange (
        0.0,
        24.0,
        0.1);

    pitchDropSlider.setNormalDonkRange (0.0, 24.0);

    pitchDropSlider.setValue (
        static_cast<double> (
            audioProcessor.getPitchDrop()),
        juce::dontSendNotification);

    pitchDropSlider.setTooltip ("CYKA: controls the initial pitch drop.");

    pitchDropSlider.onValueChange = [this]()
        {
            audioProcessor.setPitchDrop (
                static_cast<float> (pitchDropSlider.getValue()));

        };

    setupSlider (
        decaySlider,
        decayLabel,
        "BLYAT");

    decaySlider.setRange (
        0.08,
        1.0,
        0.01);

    decaySlider.setNormalDonkRange (0.08, 1.0);

    decaySlider.setValue (
        static_cast<double> (
            audioProcessor.getDecay()),
        juce::dontSendNotification);

    decaySlider.setTooltip ("BLYAT: controls how long the donk hangs around.");

    decaySlider.onValueChange = [this]()
        {
            audioProcessor.setDecay (
                static_cast<float> (decaySlider.getValue()));

        };

    setupSlider (
        knockSlider,
        knockLabel,
        "DONK");

    knockSlider.setRange (
        0.0,
        6.0,
        0.01);

    knockSlider.setNormalDonkRange (0.0, 6.0);

    knockSlider.setValue (
        static_cast<double> (
            audioProcessor.getKnock()),
        juce::dontSendNotification);

    knockSlider.setTooltip ("DONK: controls the metallic FM knock.");

    knockSlider.onValueChange = [this]()
        {
            audioProcessor.setKnock (
                static_cast<float> (knockSlider.getValue()));

        };

    setupSlider (
        ratioSlider,
        ratioLabel,
        "GOPNIK");

    ratioSlider.setRange (
        0.5,
        4.0,
        0.01);

    ratioSlider.setNormalDonkRange (0.5, 4.0);

    ratioSlider.setValue (
        static_cast<double> (
            audioProcessor.getRatio()),
        juce::dontSendNotification);

    ratioSlider.setTooltip ("GOPNIK: changes the modulation character.");

    ratioSlider.onValueChange = [this]()
        {
            audioProcessor.setRatio (
                static_cast<float> (ratioSlider.getValue()));

        };

    setupSlider (
        shapeSlider,
        shapeLabel,
        "KVASS");

    shapeSlider.setRange (
        0.0,
        1.0,
        0.01);

    shapeSlider.setNormalDonkRange (0.0, 1.0);

    shapeSlider.setValue (
        static_cast<double> (
            audioProcessor.getShape()),
        juce::dontSendNotification);

    shapeSlider.setTooltip ("KVASS: morphs the waveform.");

    shapeSlider.onValueChange = [this]()
        {
            audioProcessor.setShape (
                static_cast<float> (shapeSlider.getValue()));

        };

    setupSlider (
        toneSlider,
        toneLabel,
        "SEMECHKI");

    toneSlider.setRange (
        0.0,
        1.0,
        0.01);

    toneSlider.setNormalDonkRange (0.0, 1.0);

    toneSlider.setValue (
        static_cast<double> (
            audioProcessor.getTone()),
        juce::dontSendNotification);

    toneSlider.setTooltip ("SEMECHKI: controls brightness.");

    toneSlider.onValueChange = [this]()
        {
            audioProcessor.setTone (
                static_cast<float> (toneSlider.getValue()));

        };

    setupSlider (
        bodySlider,
        bodyLabel,
        "HARD");

    bodySlider.setRange (
        0.0,
        1.0,
        0.01);

    bodySlider.setNormalDonkRange (0.0, 1.0);

    bodySlider.setValue (
        static_cast<double> (
            audioProcessor.getBody()),
        juce::dontSendNotification);

    bodySlider.setTooltip ("HARD: controls body and sustain.");

    bodySlider.onValueChange = [this]()
        {
            audioProcessor.setBody (
                static_cast<float> (bodySlider.getValue()));

        };

    setupSlider (
        driveSlider,
        driveLabel,
        "BASS");

    driveSlider.setRange (
        1.0,
        8.0,
        0.01);

    driveSlider.setNormalDonkRange (1.0, 8.0);

    driveSlider.setValue (
        static_cast<double> (
            audioProcessor.getDrive()),
        juce::dontSendNotification);

    driveSlider.setTooltip ("BASS: controls saturation and heavy harmonics.");

    driveSlider.onValueChange = [this]()
        {
            audioProcessor.setDrive (
                static_cast<float> (driveSlider.getValue()));

        };

    declickerToggle.setButtonText ("DECLICKER");

    declickerToggle.setToggleState (
        audioProcessor.getDeclickerEnabled(),
        juce::dontSendNotification);

    declickerToggle.setTooltip ("Smooth very short note retriggers to reduce click sounds.");

    declickerToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    declickerToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    declickerToggle.setWantsKeyboardFocus (false);
    declickerToggle.setMouseClickGrabsKeyboardFocus (false);

    declickerToggle.onClick = [this]()
        {
            audioProcessor.setDeclickerEnabled (declickerToggle.getToggleState());
            const bool showStrength = declickerToggle.getToggleState();
            declickerStrengthLabel.setVisible (showStrength);
            declickerStrengthSlider.setVisible (showStrength);
        };

    addAndMakeVisible (declickerToggle);
    declickerStrengthSlider.setSliderStyle (juce::Slider::LinearHorizontal);

    declickerStrengthSlider.setRange (
        0.0,
        100.0,
        1.0);

    declickerStrengthSlider.setTextBoxStyle (
        juce::Slider::TextBoxRight,
        false,
        46,
        18);

    declickerStrengthSlider.setValue (
        audioProcessor.getDeclickerStrength() * 100.0f,
        juce::dontSendNotification);

    declickerStrengthSlider.setTooltip (
        "DECLICKER STRENGTH: higher values use a longer, more aggressive smoothing crossfade.");

    declickerStrengthSlider.setWantsKeyboardFocus (false);
    declickerStrengthSlider.setMouseClickGrabsKeyboardFocus (false);

    declickerStrengthSlider.onValueChange = [this]()
        {
            audioProcessor.setDeclickerStrength (static_cast<float> (declickerStrengthSlider.getValue()
                    / 100.0));

        };

    declickerStrengthLabel.setText (
        "STRENGTH",
        juce::dontSendNotification);

    declickerStrengthLabel.setJustificationType (juce::Justification::centredLeft);

    declickerStrengthLabel.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (9.0f)
                .withStyle ("Bold")));

    declickerStrengthLabel.setColour (
        juce::Label::textColourId,
        juce::Colour (0xffffd34e));

    addAndMakeVisible (declickerStrengthSlider);
    addAndMakeVisible (declickerStrengthLabel);
    const bool showDeclickerStrength = audioProcessor.getDeclickerEnabled();
    declickerStrengthSlider.setVisible (showDeclickerStrength);
    declickerStrengthLabel.setVisible (showDeclickerStrength);

    setupSlider (
        reverbSlider,
        reverbLabel,
        "REVERB");

    reverbSlider.setRange (
        0.0,
        100.0,
        1.0);

    reverbSlider.setValue (
        static_cast<double> (
            audioProcessor.getReverbMix() * 100.0f),
        juce::dontSendNotification);

    reverbSlider.setTooltip ("REVERB: controls the amount of reverb tail.");

    reverbSlider.onValueChange = [this]()
        {
            audioProcessor.setReverbMix (
                static_cast<float> (reverbSlider.getValue() / 100.0));

        };

    reverbToggle.setButtonText ("REVERB");

    reverbToggle.setToggleState (
        audioProcessor.getReverbEnabled(),
        juce::dontSendNotification);

    reverbToggle.setTooltip ("Turn the reverb tail ON or OFF.");

    reverbToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colour (0xffffd34e));

    reverbToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    reverbToggle.setWantsKeyboardFocus (false);
    reverbToggle.setMouseClickGrabsKeyboardFocus (false);

    reverbToggle.onClick = [this]()
        {
            audioProcessor.setReverbEnabled (reverbToggle.getToggleState());
            const bool showReverb = reverbToggle.getToggleState();
            reverbLabel.setVisible (showReverb);
            reverbSlider.setVisible (showReverb);

            sidechainAfterReverbToggle.setVisible (
                showReverb
                && sidechainToggle.getToggleState());

        };

    addAndMakeVisible (reverbToggle);
    reverbLabel.setVisible (audioProcessor.getReverbEnabled());
    reverbSlider.setVisible (audioProcessor.getReverbEnabled());
    sidechainToggle.setButtonText ("SIDECHAIN");

    sidechainToggle.setToggleState (
        audioProcessor.getSidechainEnabled(),
        juce::dontSendNotification);

    sidechainToggle.setTooltip ("Internal duck. POSITION moves the deepest dip through the donk.");

    sidechainToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    sidechainToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    sidechainToggle.setWantsKeyboardFocus (false);
    sidechainToggle.setMouseClickGrabsKeyboardFocus (false);
    sidechainAfterReverbToggle.setButtonText ("AFTER REVERB");

    sidechainAfterReverbToggle.setToggleState (
        audioProcessor.getSidechainAfterReverb(),
        juce::dontSendNotification);

    sidechainAfterReverbToggle.setTooltip ("OFF = sidechain before reverb. ON = sidechain after reverb.");

    sidechainAfterReverbToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    sidechainAfterReverbToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    sidechainAfterReverbToggle.setWantsKeyboardFocus (false);
    sidechainAfterReverbToggle.setMouseClickGrabsKeyboardFocus (false);

    sidechainAfterReverbToggle.onClick = [this]()
        {
            audioProcessor.setSidechainAfterReverb (sidechainAfterReverbToggle.getToggleState());
        };

    addAndMakeVisible (sidechainAfterReverbToggle);

    const auto setupSidechainSlider = [this] (
            PercentSlider& slider,
            juce::Label& label,
            const juce::String& text)
        {
            slider.setSliderStyle (juce::Slider::LinearHorizontal);

            slider.setRange (
                0.0,
                100.0,
                1.0);

            slider.setTextBoxStyle (
                juce::Slider::TextBoxRight,
                false,
                46,
                18);

            slider.setWantsKeyboardFocus (false);
            slider.setMouseClickGrabsKeyboardFocus (false);

            slider.setColour (
                juce::Slider::trackColourId,
                juce::Colour (0xff344348));

            slider.setColour (
                juce::Slider::thumbColourId,
                juce::Colour (0xffffe580));

            slider.setColour (
                juce::Slider::textBoxTextColourId,
                juce::Colours::white);

            slider.setColour (
                juce::Slider::textBoxBackgroundColourId,
                juce::Colour (0xff111019));

            slider.setColour (
                juce::Slider::textBoxOutlineColourId,
                juce::Colour (0xff494458));

            label.setText (
                text,
                juce::dontSendNotification);

            label.setJustificationType (juce::Justification::centredLeft);

            label.setFont (juce::Font (juce::FontOptions()
                        .withName ("Arial")
                        .withHeight (9.0f)
                        .withStyle ("Bold")));

            label.setColour (
                juce::Label::textColourId,
                juce::Colour (0xffffd34e));

            addAndMakeVisible (slider);
            addAndMakeVisible (label);
        };

    setupSidechainSlider (
        sidechainAmountSlider,
        sidechainAmountLabel,
        "AMOUNT");

    setupSidechainSlider (
        sidechainPositionSlider,
        sidechainPositionLabel,
        "POSITION");

    sidechainAmountSlider.setValue (
        audioProcessor.getSidechainAmount() * 100.0f,
        juce::dontSendNotification);

    sidechainPositionSlider.setValue (
        audioProcessor.getSidechainPosition() * 100.0f,
        juce::dontSendNotification);

    sidechainAmountSlider.onValueChange = [this]()
        {
            audioProcessor.setSidechainAmount (static_cast<float> (sidechainAmountSlider.getValue()
                    / 100.0));

        };

    sidechainPositionSlider.onValueChange = [this]()
        {
            audioProcessor.setSidechainPosition (static_cast<float> (sidechainPositionSlider.getValue()
                    / 100.0));

        };

    sidechainToggle.onClick = [this]()
        {
            audioProcessor.setSidechainEnabled (sidechainToggle.getToggleState());
            const bool showControls = sidechainToggle.getToggleState();
            sidechainAmountLabel.setVisible (showControls);
            sidechainAmountSlider.setVisible (showControls);
            sidechainPositionLabel.setVisible (showControls);
            sidechainPositionSlider.setVisible (showControls);

            sidechainAfterReverbToggle.setVisible (
                showControls
                && reverbToggle.getToggleState());

        };

    addAndMakeVisible (sidechainToggle);
    const bool showSidechainControls = audioProcessor.getSidechainEnabled();
    sidechainAmountLabel.setVisible (showSidechainControls);
    sidechainAmountSlider.setVisible (showSidechainControls);
    sidechainPositionLabel.setVisible (showSidechainControls);
    sidechainPositionSlider.setVisible (showSidechainControls);

    sidechainAfterReverbToggle.setVisible (
        showSidechainControls
        && audioProcessor.getReverbEnabled());

    setupSlider (
        volumeSlider,
        volumeLabel,
        "VOLUME");

    volumeSlider.setRange (
        0.0,
        100.0,
        1.0);

    volumeSlider.setValue (
        static_cast<double> (
            audioProcessor.getVolume() * 100.0f),
        juce::dontSendNotification);

    volumeSlider.setTooltip ("VOLUME: controls the final donk volume.");

    volumeSlider.onValueChange = [this]()
        {
            audioProcessor.setVolume (
                static_cast<float> (volumeSlider.getValue() / 100.0));

        };

    setupButton (
        randomButton,
        "RANDOM DONK");

    randomButton.setTooltip ("Create a new random donk. Reverb and Donk Loop stay exactly as they are.");

    randomButton.onClick = [this]()
        {
            randomize();
        };

    basslineToggle.setButtonText ("BASSLINE");

    basslineToggle.setToggleState (
        audioProcessor.getBasslineEnabled(),
        juce::dontSendNotification);

    basslineToggle.setTooltip ("ON = normal StarDonk. OFF = pure donk.");

    basslineToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    basslineToggle.setWantsKeyboardFocus (false);
    basslineToggle.setMouseClickGrabsKeyboardFocus (false);

    basslineToggle.onClick = [this]()
        {
            audioProcessor.setBasslineEnabled (basslineToggle.getToggleState());
        };

    addAndMakeVisible (basslineToggle);

    donkLoopSpeedLabel.setText (
        "DONK LOOP SPEED",
        juce::dontSendNotification);

    donkLoopSpeedLabel.setJustificationType (juce::Justification::centredLeft);

    donkLoopSpeedLabel.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (9.0f)
                .withStyle ("Bold")));

    donkLoopSpeedLabel.setColour (
        juce::Label::textColourId,
        juce::Colour (0xffffd34e));

    addAndMakeVisible (donkLoopSpeedLabel);
    donkLoopSpeedSlider.setSliderStyle (juce::Slider::LinearHorizontal);

    donkLoopSpeedSlider.setRange (
        0.05,
        1.00,
        0.01);

    donkLoopSpeedSlider.setValue (
        audioProcessor.getDonkLoopSpeed(),
        juce::dontSendNotification);

    donkLoopSpeedSlider.setTextValueSuffix (" s");
    donkLoopSpeedSlider.setNumDecimalPlacesToDisplay (2);

    donkLoopSpeedSlider.setTextBoxStyle (
        juce::Slider::TextBoxRight,
        false,
        58,
        18);

    donkLoopSpeedSlider.setTooltip (
        "Seconds between DONK LOOP hits. 1.00 s is the standard and slowest speed. 0.05 s is maximum blyat speed.");

    donkLoopSpeedSlider.setWantsKeyboardFocus (false);
    donkLoopSpeedSlider.setMouseClickGrabsKeyboardFocus (false);

    donkLoopSpeedSlider.setColour (
        juce::Slider::trackColourId,
        juce::Colour (0xff344348));

    donkLoopSpeedSlider.setColour (
        juce::Slider::thumbColourId,
        juce::Colour (0xffffe580));

    donkLoopSpeedSlider.setColour (
        juce::Slider::textBoxTextColourId,
        juce::Colours::white);

    donkLoopSpeedSlider.setColour (
        juce::Slider::textBoxBackgroundColourId,
        juce::Colour (0xff111019));

    donkLoopSpeedSlider.setColour (
        juce::Slider::textBoxOutlineColourId,
        juce::Colour (0xff494458));

    donkLoopSpeedSlider.onValueChange = [this]()
        {
            audioProcessor.setDonkLoopSpeed (
                static_cast<float> (donkLoopSpeedSlider.getValue()));

        };

    addAndMakeVisible (donkLoopSpeedSlider);

    setupButton (
        donkLoopButton,
        "DONK LOOP");

    donkLoopButton.setTooltip ("Automatically test the current donk at the speed above.");

    donkLoopButton.onClick = [this]()
        {
            const bool newState = ! audioProcessor.getDonkLoopEnabled();
            audioProcessor.setDonkLoopEnabled (newState);
            updateLoopButton();
        };

    updateLoopButton();
    experimentalControlToggle.setButtonText ("EXPERIMENTAL CONTROL");

    experimentalControlToggle.setToggleState (
        audioProcessor.getExperimentalControlEnabled(),
        juce::dontSendNotification);

    experimentalControlToggle.setTooltip ("Unlock extremely wide knob ranges for experimental sounds.");

    experimentalControlToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    experimentalControlToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    experimentalControlToggle.setWantsKeyboardFocus (false);
    experimentalControlToggle.setMouseClickGrabsKeyboardFocus (false);

    experimentalControlToggle.onClick = [this]()
        {
            audioProcessor.setExperimentalControlEnabled (experimentalControlToggle.getToggleState());
            applyExperimentalSliderRanges();
            updateExperimentalControls();
            updateSlidersFromProcessor();
            repaint();
        };

    addAndMakeVisible (experimentalControlToggle);

    // experiment random grejen
    experimentalRandomToggle.setButtonText ("EXPERIMENTAL RANDOM");

    experimentalRandomToggle.setToggleState (
        audioProcessor.getExperimentalRandomEnabled(),
        juce::dontSendNotification);

    experimentalRandomToggle.setTooltip ("Allow RANDOM DONK to use the full experimental ranges.");

    experimentalRandomToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    experimentalRandomToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    experimentalRandomToggle.setWantsKeyboardFocus (false);
    experimentalRandomToggle.setMouseClickGrabsKeyboardFocus (false);

    experimentalRandomToggle.onClick = [this]()
        {
            audioProcessor.setExperimentalRandomEnabled (experimentalRandomToggle.getToggleState());
            updateExperimentalControls();
        };

    addAndMakeVisible (experimentalRandomToggle);
    applyExperimentalSliderRanges();
    updateExperimentalControls();
    updateSlidersFromProcessor();

    setupButton (
        refreshPresetButton,
        "R");

    refreshPresetButton.setTooltip ("Refresh the .donk presets from Documents/StarDonk/Presets.");

    refreshPresetButton.onClick = [this]()
        {
            refreshPresetsFromDisk();
        };

    setupButton (
        previousPresetButton,
        "<");

    previousPresetButton.setTooltip ("Previous preset.");

    previousPresetButton.onClick = [this]()
        {
            previousPreset();
        };

    setupButton (
        nextPresetButton,
        ">");

    nextPresetButton.setTooltip ("Next preset.");

    nextPresetButton.onClick = [this]()
        {
            nextPreset();
        };

    setupButton (
        savePresetButton,
        "SAVE");

    savePresetButton.setTooltip ("Save this sound as a .donk preset.");

    savePresetButton.onClick = [this]()
        {
            savePreset();
        };

    animationsOffToggle.setButtonText ("TURN OFF ANIMATIONS");

    animationsOffToggle.setToggleState (
        audioProcessor.getAnimationsDisabled(),
        juce::dontSendNotification);

    animationsOffToggle.setTooltip ("Stop the fancy StarDonk animations.");

    animationsOffToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);

    animationsOffToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));

    animationsOffToggle.setWantsKeyboardFocus (false);
    animationsOffToggle.setMouseClickGrabsKeyboardFocus (false);

    animationsOffToggle.onClick = [this]()
        {
            audioProcessor.setAnimationsDisabled (animationsOffToggle.getToggleState());

            if (audioProcessor.getAnimationsDisabled())
            {
                starEnergy = 0.0f;
                faceBounce = 0.0f;
            }

            repaint();
        };

    addAndMakeVisible (animationsOffToggle);
    minimalisticToggle.setButtonText ("MINIMALISTIC");
    minimalisticToggle.setToggleState (
        audioProcessor.getMinimalisticEnabled(),
        juce::dontSendNotification);
    minimalisticToggle.setTooltip ("Black and white, no fancy stuff, real knob values.");
    minimalisticToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);
    minimalisticToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colours::white);
    minimalisticToggle.setWantsKeyboardFocus (false);
    minimalisticToggle.setMouseClickGrabsKeyboardFocus (false);
    minimalisticToggle.onClick = [this]()
    {
        audioProcessor.setMinimalisticEnabled (minimalisticToggle.getToggleState());

        if (minimalisticToggle.getToggleState())
        {
            if (experimentalFunToggle.getToggleState())
            {
                experimentalFunToggle.setToggleState (
                    false,
                    juce::dontSendNotification);
                disableAllExperimentalFun();
            }

            funPanel.setVisible (false);
            starEnergy = 0.0f;
            faceBounce = 0.0f;
        }

        applyMinimalisticMode();
        resized();
        repaint();
    };
    addAndMakeVisible (minimalisticToggle);
    advancedModeToggle.setButtonText ("ADVANCED MODE");
    advancedModeToggle.setToggleState (
        audioProcessor.getAdvancedModeEnabled(),
        juce::dontSendNotification);
    advancedModeToggle.setTooltip (
        "Show the full donk engine: carrier, four FM operators, envelopes, resonant filter, distortion and optional sub.");
    advancedModeToggle.setColour (
        juce::ToggleButton::textColourId,
        juce::Colours::white);
    advancedModeToggle.setColour (
        juce::ToggleButton::tickColourId,
        juce::Colour (0xffffc933));
    advancedModeToggle.setWantsKeyboardFocus (false);
    advancedModeToggle.setMouseClickGrabsKeyboardFocus (false);
    advancedModeToggle.onClick = [this]()
    {
        audioProcessor.setAdvancedModeEnabled (advancedModeToggle.getToggleState());

        if (advancedModeToggle.getToggleState())
            setSize (920, 780);
        else
            setSize (720, 610);

        updateAdvancedMode();
        applyMinimalisticMode();
        resized();
        repaint();
    };
    addAndMakeVisible (advancedModeToggle);
    experimentalFunToggle.setButtonText ("EXPERIMENTAL FUN");
    experimentalFunToggle.setColour (juce::ToggleButton::textColourId, juce::Colours::white);
    experimentalFunToggle.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xffffc933));
    experimentalFunToggle.setWantsKeyboardFocus (false);
    experimentalFunToggle.setMouseClickGrabsKeyboardFocus (false);
    experimentalFunToggle.setTooltip ("Opens the completely unnecessary fun department.");
    experimentalFunToggle.onClick = [this]()
    {
        if (! experimentalFunToggle.getToggleState())
            disableAllExperimentalFun();

        updateExperimentalFunVisibility();
        resized();
        repaint();
    };
    addAndMakeVisible (experimentalFunToggle);
    addChildComponent (funPanel);

    const auto setupFunToggle = [this] (juce::ToggleButton& toggle, const juce::String& text)
    {
        toggle.setButtonText (text);
        toggle.setColour (juce::ToggleButton::textColourId, juce::Colours::white);
        toggle.setColour (juce::ToggleButton::tickColourId, juce::Colour (0xffffc933));
        toggle.setWantsKeyboardFocus (false);
        toggle.setMouseClickGrabsKeyboardFocus (false);
        funPanel.addAndMakeVisible (toggle);
    };
    setupFunToggle (semechkiRainToggle, "SEMECHKI RAIN");
    setupFunToggle (babushkaSafeToggle, "BABUSHKA SAFE MODE");
    setupFunToggle (neighborMeterToggle, "NEIGHBOR COMPLAINTS METER");
    setupFunToggle (redAlertToggle, "RED ALERT MODE");
    setupFunToggle (impactCracksToggle, "DONK IMPACT CRACKS");
    setupFunToggle (bassEarthquakeToggle, "BASS EARTHQUAKE");
    setupFunToggle (screenShakeToggle, "SCREEN SHAKE");
    setupFunToggle (faceSquashToggle, "WIDE BOI");
    setupFunToggle (ladaModeToggle, "LADA MODE");
    setupFunToggle (discoModeToggle, "DISCO MODE");
    setupFunToggle (flyingPresetToggle, "PRESET NAMES FLYING IN");
    babushkaSafeToggle.setToggleState (audioProcessor.getBabushkaSafeMode(), juce::dontSendNotification);
    babushkaSafeToggle.onClick = [this]()
    {
        audioProcessor.setBabushkaSafeMode (babushkaSafeToggle.getToggleState());
    };

    bassEarthquakeToggle.onClick = [this]()
    {
        if (bassEarthquakeToggle.getToggleState())
        {
            earthquakeSnapshot = captureDonkSnapshot();
            earthquakeHasSnapshot = true;
        }
        else
        {
            restoreBassEarthquake();
        }
    };
    setupButton (randomExplosionButton, "RANDOM BUTTON EXPLOSION");
    setupButton (doNotPressButton, "DO NOT PRESS");
    setupButton (memeButton, "MEME");
    setupButton (closeFunPanelButton, "X");
    funPanel.addAndMakeVisible (randomExplosionButton);
    funPanel.addAndMakeVisible (doNotPressButton);
    funPanel.addAndMakeVisible (memeButton);
    funPanel.addAndMakeVisible (closeFunPanelButton);
    closeFunPanelButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff211b2c));
    closeFunPanelButton.setColour (juce::TextButton::textColourOffId, juce::Colour (0xffffd34e));
    closeFunPanelButton.onClick = [this]() { closeExperimentalFunPanel(); };
    doNotPressButton.setColour (juce::TextButton::buttonColourId, juce::Colour (0xff7b1414));
    doNotPressButton.setColour (juce::TextButton::textColourOffId, juce::Colours::white);
    randomExplosionButton.onClick = [this]() { triggerRandomExplosion(); };
    doNotPressButton.onClick = [this]() { startDoNotPress(); };
    memeButton.onClick = [this]()
    {
        memePanelOpen = ! memePanelOpen;
        updateMemeVisibility();
        resized();
    };
    setupButton (giveBackDonkButton, "GIVE BACK DONK");
    giveBackDonkButton.onClick = [this]() { giveBackDonk(); };

    setupButton (
        helpButton,
        "?");

    helpButton.setTooltip ("Quick StarDonk instructions and links.");

    helpButton.onClick = [this]()
        {
            showHelpPanel();
        };

    setupButton (
        presetBox,
        "SELECT DONK");

    presetBox.setTooltip ("Select a .donk preset.");

    presetBox.onClick = [this]()
        {
            if (presetGrid != nullptr
                &&
                presetGrid->isVisible())
            {
                hidePresetGrid();
            }
            else
            {
                showPresetGrid();
            }
        };

    refreshPresetBox();

    if (audioProcessor.getAdvancedModeEnabled())
        setSize (920, 780);
    else
        setSize (720, 610);

    startTimerHz (60);
}

NewProjectAudioProcessorEditor::
~NewProjectAudioProcessorEditor()
{
    stopTimer();
}

void NewProjectAudioProcessorEditor::setupSlider (
    juce::Slider& slider,
    juce::Label& label,
    const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setWantsKeyboardFocus (false);
    slider.setMouseClickGrabsKeyboardFocus (false);

    slider.setTextBoxStyle (
        juce::Slider::TextBoxBelow,
        true,
        120,
        21);

    slider.setColour (
        juce::Slider::rotarySliderFillColourId,
        juce::Colour (0xffffcc33));

    slider.setColour (
        juce::Slider::rotarySliderOutlineColourId,
        juce::Colour (0xff454052));

    slider.setColour (
        juce::Slider::thumbColourId,
        juce::Colour (0xffffe580));

    slider.setColour (
        juce::Slider::textBoxTextColourId,
        juce::Colours::white);

    slider.setColour (
        juce::Slider::textBoxBackgroundColourId,
        juce::Colour (0xff111019));

    slider.setColour (
        juce::Slider::textBoxOutlineColourId,
        juce::Colour (0xff494458));

    slider.setDoubleClickReturnValue (
        true,
        0.0);

    addAndMakeVisible (slider);

    label.setText (
        labelText,
        juce::dontSendNotification);

    label.setJustificationType (juce::Justification::centred);

    label.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (15.0f)
                .withStyle ("Bold")));

    label.setColour (
        juce::Label::textColourId,
        juce::Colour (0xffffd34e));

    addAndMakeVisible (label);
}

void NewProjectAudioProcessorEditor::setupButton (juce::TextButton& button, const juce::String& text)
{
    button.setButtonText (text);
    button.setWantsKeyboardFocus (false);
    button.setMouseClickGrabsKeyboardFocus (false);

    button.setColour (
        juce::TextButton::buttonColourId,
        juce::Colour (0xff211b2c));

    button.setColour (
        juce::TextButton::buttonOnColourId,
        juce::Colour (0xffffc933));

    button.setColour (
        juce::TextButton::textColourOffId,
        juce::Colour (0xffffd34e));

    button.setColour (
        juce::TextButton::textColourOnId,
        juce::Colour (0xff08070d));

    addAndMakeVisible (button);
}

void NewProjectAudioProcessorEditor::setupAdvancedControls()
{
    for (int i = 0;
         i < NewProjectAudioProcessor::AdvancedParameterCount;
         ++i)
    {
        auto& slider = advancedSliders[static_cast<size_t> (i)];
        auto& label = advancedLabels[static_cast<size_t> (i)];
        slider.setSliderStyle (juce::Slider::LinearHorizontal);

        slider.setTextBoxStyle (
            juce::Slider::TextBoxRight,
            false,
            58,
            18);

        slider.setRange (
            audioProcessor.getAdvancedParameterMinimum (i),
            audioProcessor.getAdvancedParameterMaximum (i),
            audioProcessor.getAdvancedParameterInterval (i));

        slider.setValue (
            audioProcessor.getAdvancedParameterValue (i),
            juce::dontSendNotification);

        slider.setDoubleClickReturnValue (
            true,
            audioProcessor.getAdvancedParameterDefault (i));

        slider.setTooltip (audioProcessor.getAdvancedParameterName (i));
        slider.setWantsKeyboardFocus (false);
        slider.setMouseClickGrabsKeyboardFocus (false);

        slider.onValueChange = [this, i]()
            {
                audioProcessor.setAdvancedParameterValue (
                    i,
                    static_cast<float> (advancedSliders[static_cast<size_t> (i)].getValue()));

            };

        label.setText (
            audioProcessor.getAdvancedParameterName (i).toUpperCase(),
            juce::dontSendNotification);

        label.setJustificationType (juce::Justification::centredLeft);

        label.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (9.0f)
                    .withStyle ("Bold")));

        label.setInterceptsMouseClicks (
            false,
            false);

        addAndMakeVisible (slider);
        addAndMakeVisible (label);
    }
}

void NewProjectAudioProcessorEditor::updateAdvancedControlsFromProcessor()
{
    for (int i = 0;
         i < NewProjectAudioProcessor::AdvancedParameterCount;
         ++i)
    {
        advancedSliders[static_cast<size_t> (i)].setValue (
            audioProcessor.getAdvancedParameterValue (i),
            juce::dontSendNotification);
    }
}

void NewProjectAudioProcessorEditor::updateAdvancedMode()
{
    const bool advanced = audioProcessor.getAdvancedModeEnabled();

    advancedModeToggle.setToggleState (
        advanced,
        juce::dontSendNotification);

    DonkSlider* macroSliders[] =
    {
        &pitchDropSlider,
        &decaySlider,
        &knockSlider,
        &ratioSlider,
        &shapeSlider,
        &toneSlider,
        &bodySlider,
        &driveSlider
    };

    for (auto* slider : macroSliders)
    {
        slider->setSliderStyle (
            advanced
                ? juce::Slider::LinearHorizontal
                : juce::Slider::RotaryHorizontalVerticalDrag);

        slider->setTextBoxStyle (
            advanced
                ? juce::Slider::TextBoxRight
                : juce::Slider::TextBoxBelow,
            false,
            advanced ? 58 : 120,
            advanced ? 18 : 20);
    }

    for (int i = 0;
         i < NewProjectAudioProcessor::AdvancedParameterCount;
         ++i)
    {
        const bool legacyClassic = i >= NewProjectAudioProcessor::ClassicBlend
            && i <= NewProjectAudioProcessor::ClassicELevel;

        advancedSliders[static_cast<size_t> (i)].setVisible (advanced && ! legacyClassic);
        advancedLabels[static_cast<size_t> (i)].setVisible (advanced && ! legacyClassic);
    }

    pitchDropLabel.setText (
        advanced ? "PITCH DROP" : "CYKA",
        juce::dontSendNotification);

    decayLabel.setText (
        advanced ? "DECAY MACRO" : "BLYAT",
        juce::dontSendNotification);

    knockLabel.setText (
        advanced ? "FM1 AMOUNT" : "DONK",
        juce::dontSendNotification);

    ratioLabel.setText (
        advanced ? "FM1 RATIO" : "GOPNIK",
        juce::dontSendNotification);

    shapeLabel.setText (
        advanced ? "CARRIER SHAPE" : "KVASS",
        juce::dontSendNotification);

    toneLabel.setText (
        advanced ? "FILTER CUTOFF" : "SEMECHKI",
        juce::dontSendNotification);

    bodyLabel.setText (
        advanced ? "BODY MACRO" : "HARD",
        juce::dontSendNotification);

    driveLabel.setText (
        advanced ? "DRIVE" : "BASS",
        juce::dontSendNotification);

    experimentalControlToggle.setVisible (true);
    experimentalRandomToggle.setVisible (audioProcessor.getExperimentalControlEnabled());
    giveBackDonkButton.setVisible (! advanced);
    updateAdvancedControlsFromProcessor();
}

void NewProjectAudioProcessorEditor::applyMinimalisticMode()
{
    const bool minimal = audioProcessor.getMinimalisticEnabled();
    const bool advanced = audioProcessor.getAdvancedModeEnabled();

    minimalisticToggle.setToggleState (
        minimal,
        juce::dontSendNotification);

    pitchDropSlider.setRawValueDisplay (minimal || advanced);
    decaySlider.setRawValueDisplay (minimal || advanced);
    knockSlider.setRawValueDisplay (minimal || advanced);
    ratioSlider.setRawValueDisplay (minimal || advanced);
    shapeSlider.setRawValueDisplay (minimal || advanced);
    toneSlider.setRawValueDisplay (minimal || advanced);
    bodySlider.setRawValueDisplay (minimal || advanced);
    driveSlider.setRawValueDisplay (minimal || advanced);

    // göm skämt labels o pynt grejer i minimal mode
    pitchDropLabel.setVisible (! minimal || advanced);
    decayLabel.setVisible (! minimal || advanced);
    knockLabel.setVisible (! minimal || advanced);
    ratioLabel.setVisible (! minimal || advanced);
    shapeLabel.setVisible (! minimal || advanced);
    toneLabel.setVisible (! minimal || advanced);
    bodyLabel.setVisible (! minimal || advanced);
    driveLabel.setVisible (! minimal || advanced);
    animationsOffToggle.setVisible (! minimal);
    experimentalFunToggle.setVisible (! minimal);
    funPanel.setVisible (! minimal
                        && experimentalFunToggle.getToggleState());

    const auto fg = minimal
            ? juce::Colours::white
            : juce::Colour (0xffffd34e);

    const auto knobFill = minimal
            ? juce::Colours::white
            : juce::Colour (0xffffcc33);

    const auto knobOutline = minimal
            ? juce::Colour (0xff555555)
            : juce::Colour (0xff454052);

    const auto thumb = minimal
            ? juce::Colours::white
            : juce::Colour (0xffffe580);

    const auto textBoxBackground = minimal
            ? juce::Colours::black
            : juce::Colour (0xff111019);

    const auto textBoxOutline = minimal
            ? juce::Colour (0xff777777)
            : juce::Colour (0xff494458);

    juce::Slider* sliders[] =
    {
        &pitchDropSlider, &decaySlider, &knockSlider, &ratioSlider,
        &shapeSlider, &toneSlider, &bodySlider, &driveSlider,
        &reverbSlider, &declickerStrengthSlider,
        &sidechainAmountSlider, &sidechainPositionSlider,
        &volumeSlider, &donkLoopSpeedSlider
    };

    for (auto* slider : sliders)
    {
        slider->setColour (
            juce::Slider::rotarySliderFillColourId,
            knobFill);
        slider->setColour (
            juce::Slider::rotarySliderOutlineColourId,
            knobOutline);
        slider->setColour (
            juce::Slider::thumbColourId,
            thumb);
        slider->setColour (
            juce::Slider::textBoxTextColourId,
            juce::Colours::white);
        slider->setColour (
            juce::Slider::textBoxBackgroundColourId,
            textBoxBackground);
        slider->setColour (
            juce::Slider::textBoxOutlineColourId,
            textBoxOutline);
    }

    juce::Label* labels[] =
    {
        &reverbLabel,
        &declickerStrengthLabel,
        &donkLoopSpeedLabel,
        &sidechainAmountLabel,
        &sidechainPositionLabel,
        &volumeLabel
    };

    for (auto* label : labels)
        label->setColour (
            juce::Label::textColourId,
            fg);

    juce::TextButton* buttons[] =
    {
        &randomButton,
        &donkLoopButton,
        &refreshPresetButton,
        &presetBox,
        &previousPresetButton,
        &nextPresetButton,
        &savePresetButton,
        &helpButton,
        &giveBackDonkButton
    };

    for (auto* button : buttons)
    {
        button->setColour (
            juce::TextButton::buttonColourId,
            minimal
                ? juce::Colours::black
                : juce::Colour (0xff211b2c));
        button->setColour (juce::TextButton::buttonOnColourId, minimal ? juce::Colour (0xff222222)
                : juce::Colour (0xffffc933));
        button->setColour (
            juce::TextButton::textColourOffId,
            minimal
                ? juce::Colours::white
                : juce::Colour (0xffffd34e));
        button->setColour (
            juce::TextButton::textColourOnId,
            minimal
                ? juce::Colours::white
                : juce::Colour (0xff08070d));
    }

    juce::ToggleButton* toggles[] =
    {
        &basslineToggle,
        &reverbToggle,
        &sidechainToggle,
        &sidechainAfterReverbToggle,
        &declickerToggle,
        &experimentalControlToggle,
        &experimentalRandomToggle,
        &minimalisticToggle,
        &advancedModeToggle
    };

    for (auto* toggle : toggles)
    {
        toggle->setColour (
            juce::ToggleButton::textColourId,
            juce::Colours::white);
        toggle->setColour (
            juce::ToggleButton::tickColourId,
            minimal
                ? juce::Colours::white
                : juce::Colour (0xffffc933));
    }

    for (int i = 0;
         i < NewProjectAudioProcessor::AdvancedParameterCount;
         ++i)
    {
        auto& slider = advancedSliders[static_cast<size_t> (i)];
        auto& label = advancedLabels[static_cast<size_t> (i)];

        slider.setColour (
            juce::Slider::rotarySliderFillColourId,
            minimal
                ? juce::Colours::white
                : juce::Colour (0xffffcc33));

        slider.setColour (juce::Slider::rotarySliderOutlineColourId, minimal ? juce::Colour (0xff444444)
                : juce::Colour (0xff454052));

        slider.setColour (
            juce::Slider::thumbColourId,
            minimal
                ? juce::Colours::white
                : juce::Colour (0xffffe580));

        slider.setColour (
            juce::Slider::textBoxTextColourId,
            juce::Colours::white);

        slider.setColour (
            juce::Slider::textBoxBackgroundColourId,
            minimal
                ? juce::Colours::black
                : juce::Colour (0xff111019));

        slider.setColour (juce::Slider::textBoxOutlineColourId, minimal ? juce::Colour (0xff777777)
                : juce::Colour (0xff494458));

        label.setColour (
            juce::Label::textColourId,
            minimal
                ? juce::Colours::white
                : juce::Colour (0xffffd34e));
    }

    repaint();
}

void NewProjectAudioProcessorEditor::updateLoopButton()
{
    if (audioProcessor.getDonkLoopEnabled())
    {
        donkLoopButton.setButtonText ("STOP DONK LOOP");
    }
    else
    {
        donkLoopButton.setButtonText ("DONK LOOP");
    }
}

void NewProjectAudioProcessorEditor::updateExperimentalControls()
{
    const bool experimentalEnabled = audioProcessor.getExperimentalControlEnabled();

    experimentalControlToggle.setToggleState (
        experimentalEnabled,
        juce::dontSendNotification);

    experimentalRandomToggle.setVisible (experimentalEnabled);
    experimentalRandomToggle.setEnabled (experimentalEnabled);

    experimentalRandomToggle.setToggleState (
        experimentalEnabled
            &&
            audioProcessor.getExperimentalRandomEnabled(),
        juce::dontSendNotification);
}

void NewProjectAudioProcessorEditor::applyExperimentalSliderRanges()
{
    const bool experimentalEnabled = audioProcessor.getExperimentalControlEnabled();

    if (experimentalEnabled)
    {
        pitchDropSlider.setRange (-48.0, 72.0, 0.1);
        decaySlider.setRange     (0.01, 3.0, 0.01);
        knockSlider.setRange     (-12.0, 18.0, 0.01);
        ratioSlider.setRange     (-8.0, 12.0, 0.01);
        shapeSlider.setRange     (-1.0, 2.0, 0.01);
        toneSlider.setRange      (-0.03, 1.50, 0.01);
        bodySlider.setRange      (-0.25, 1.50, 0.01);
        driveSlider.setRange     (-8.0, 16.0, 0.01);
    }
    else
    {
        const double p = audioProcessor.getPitchDrop();
        const double d = audioProcessor.getDecay();
        const double k = audioProcessor.getKnock();
        const double r = audioProcessor.getRatio();
        const double s = audioProcessor.getShape();
        const double t = audioProcessor.getTone();
        const double b = audioProcessor.getBody();
        const double v = audioProcessor.getDrive();
        pitchDropSlider.setRange (juce::jmin (0.0, p), juce::jmax (24.0, p), 0.1);
        decaySlider.setRange     (juce::jmin (0.08, d), juce::jmax (1.0, d), 0.01);
        knockSlider.setRange     (juce::jmin (0.0, k), juce::jmax (6.0, k), 0.01);
        ratioSlider.setRange     (juce::jmin (0.5, r), juce::jmax (4.0, r), 0.01);
        shapeSlider.setRange     (juce::jmin (0.0, s), juce::jmax (1.0, s), 0.01);
        toneSlider.setRange      (juce::jmin (0.0, t), juce::jmax (1.0, t), 0.01);
        bodySlider.setRange      (juce::jmin (0.0, b), juce::jmax (1.0, b), 0.01);
        driveSlider.setRange     (juce::jmin (1.0, v), juce::jmax (8.0, v), 0.01);
    }
}

void NewProjectAudioProcessorEditor::refreshPresetsFromDisk()
{
    hidePresetGrid();
    audioProcessor.refreshUserPresets();
    refreshPresetBox();
    updateSlidersFromProcessor();
    repaint();
}

void NewProjectAudioProcessorEditor::randomize()
{
    hidePresetGrid();
    previousRandomSnapshot = captureDonkSnapshot();
    hasPreviousRandom = true;
    audioProcessor.randomizeSound();

    if (discoModeToggle.getToggleState())
    {
        discoLife = 1.0f;
        discoHue = funRandom.nextFloat();
    }

    if (experimentalFunToggle.getToggleState())
        triggerRandomExplosion();

    updateSlidersFromProcessor();
    updateLoopButton();
    refreshPresetBox();
    repaint();
}

void NewProjectAudioProcessorEditor::
updateSlidersFromProcessor()
{
    applyExperimentalSliderRanges();

    pitchDropSlider.setValue (
        static_cast<double> (
            audioProcessor.getPitchDrop()),
        juce::dontSendNotification);

    decaySlider.setValue (
        static_cast<double> (
            audioProcessor.getDecay()),
        juce::dontSendNotification);

    knockSlider.setValue (
        static_cast<double> (
            audioProcessor.getKnock()),
        juce::dontSendNotification);

    ratioSlider.setValue (
        static_cast<double> (
            audioProcessor.getRatio()),
        juce::dontSendNotification);

    shapeSlider.setValue (
        static_cast<double> (
            audioProcessor.getShape()),
        juce::dontSendNotification);

    toneSlider.setValue (
        static_cast<double> (
            audioProcessor.getTone()),
        juce::dontSendNotification);

    bodySlider.setValue (
        static_cast<double> (
            audioProcessor.getBody()),
        juce::dontSendNotification);

    driveSlider.setValue (
        static_cast<double> (
            audioProcessor.getDrive()),
        juce::dontSendNotification);

    reverbSlider.setValue (
        static_cast<double> (
            audioProcessor.getReverbMix() * 100.0f),
        juce::dontSendNotification);

    sidechainAmountSlider.setValue (
        audioProcessor.getSidechainAmount() * 100.0f,
        juce::dontSendNotification);

    sidechainPositionSlider.setValue (
        audioProcessor.getSidechainPosition() * 100.0f,
        juce::dontSendNotification);

    declickerStrengthSlider.setValue (
        audioProcessor.getDeclickerStrength() * 100.0f,
        juce::dontSendNotification);

    volumeSlider.setValue (
        static_cast<double> (
            audioProcessor.getVolume() * 100.0f),
        juce::dontSendNotification);

    basslineToggle.setToggleState (
        audioProcessor.getBasslineEnabled(),
        juce::dontSendNotification);

    reverbToggle.setToggleState (
        audioProcessor.getReverbEnabled(),
        juce::dontSendNotification);

    sidechainToggle.setToggleState (
        audioProcessor.getSidechainEnabled(),
        juce::dontSendNotification);

    sidechainAfterReverbToggle.setToggleState (
        audioProcessor.getSidechainAfterReverb(),
        juce::dontSendNotification);

    reverbLabel.setVisible (audioProcessor.getReverbEnabled());
    reverbSlider.setVisible (audioProcessor.getReverbEnabled());
    const bool showSidechainControlsNow = audioProcessor.getSidechainEnabled();
    sidechainAmountLabel.setVisible (showSidechainControlsNow);
    sidechainAmountSlider.setVisible (showSidechainControlsNow);
    sidechainPositionLabel.setVisible (showSidechainControlsNow);
    sidechainPositionSlider.setVisible (showSidechainControlsNow);

    sidechainAfterReverbToggle.setVisible (
        showSidechainControlsNow
        && audioProcessor.getReverbEnabled());

    declickerToggle.setToggleState (
        audioProcessor.getDeclickerEnabled(),
        juce::dontSendNotification);

    const bool showDeclickerStrengthNow = audioProcessor.getDeclickerEnabled();
    declickerStrengthLabel.setVisible (showDeclickerStrengthNow);
    declickerStrengthSlider.setVisible (showDeclickerStrengthNow);
    updateLoopButton();
    updateExperimentalControls();
    updateAdvancedMode();
    updateAdvancedControlsFromProcessor();
}

void NewProjectAudioProcessorEditor::
refreshPresetBox()
{
    const int numberOfPresets = audioProcessor.getNumPresets();
    const int current = audioProcessor.getCurrentPresetIndex();

    if (current >= 0
        && current < numberOfPresets)
    {
        auto displayName = audioProcessor.getPresetName (
                current);

        if (displayName.length() > 20)
            displayName = displayName.substring (0, 17)
                + "...";

        presetBox.setButtonText (displayName);
    }
    else
    {
        presetBox.setButtonText (
            numberOfPresets > 0
                ? "SELECT DONK"
                : "NO DONKS");
    }
}

void NewProjectAudioProcessorEditor::
showPresetGrid()
{
    const int numberOfPresets = audioProcessor.getNumPresets();

    if (numberOfPresets <= 0)
    {
        hidePresetGrid();
        return;
    }

    if (presetGrid == nullptr)
    {
        presetGrid = std::make_unique<PresetGridComponent>();

        presetGrid->onPresetSelected = [this](int displayIndex)
        {
            if (! juce::isPositiveAndBelow (displayIndex, presetGridTargetIndices.size()))
                return;

            const int targetPreset = presetGridTargetIndices[displayIndex];
            const auto targetFolder = presetGridTargetFolders[displayIndex];

            if (targetPreset >= 0)
            {
                audioProcessor.loadPreset (targetPreset);
                updateSlidersFromProcessor();
                refreshPresetBox();
                            triggerFlyingPresetName();
                hidePresetGrid();
            }
            else
            {
                const juce::String folderToOpen = targetFolder;
                juce::Component::SafePointer<NewProjectAudioProcessorEditor> safeThis (this);

                juce::MessageManager::callAsync ([safeThis, folderToOpen]()
                    {
                        auto* editor = safeThis.getComponent();

                        if (editor == nullptr)
                            return;

                        editor->currentPresetFolder = folderToOpen;
                        editor->showPresetGrid();
                        editor->repaint();
                    });

                return;
            }

            repaint();
        };
        addAndMakeVisible (*presetGrid);
    }

    juce::StringArray names;
    presetGridTargetIndices.clear();
    presetGridTargetFolders.clear();
    auto cleanFolder = currentPresetFolder.replaceCharacter ('\\', '/').trimCharactersAtEnd ("/");

    if (cleanFolder.isNotEmpty())
    {
        auto folderOnDisk = juce::File::getSpecialLocation (
                juce::File::userDocumentsDirectory)
                .getChildFile ("StarDonk")
                .getChildFile ("Presets")
                .getChildFile (cleanFolder);

        bool folderStillHasContent = folderOnDisk.isDirectory();
        const juce::String prefix = cleanFolder + "/";

        for (int i = 0; i < numberOfPresets; ++i)
        {
            const auto path = audioProcessor.getPresetRelativePath (i)
                    .replaceCharacter ('\\', '/');

            const int lastSlash = path.lastIndexOfChar ('/');

            const auto folder = lastSlash >= 0
                    ? path.substring (0, lastSlash)
                    : juce::String();

            if (folder.equalsIgnoreCase (cleanFolder)
                || folder.startsWithIgnoreCase (prefix))
            {
                folderStillHasContent = true;
                break;
            }
        }

        if (! folderStillHasContent)
        {
            currentPresetFolder.clear();
            cleanFolder.clear();
        }
    }

    if (cleanFolder.isNotEmpty())
    {
        names.add ("< BACK");
        presetGridTargetIndices.add (-1);
        const int slash = cleanFolder.lastIndexOfChar ('/');
        presetGridTargetFolders.add (slash >= 0 ? cleanFolder.substring (0, slash) : juce::String());
    }

    juce::StringArray childFolders;

    // presets som ligger direkt i den här mappen
    for (int i = 0; i < numberOfPresets; ++i)
    {
        auto path = audioProcessor.getPresetRelativePath (i)
                .replaceCharacter ('\\', '/');

        const int lastSlash = path.lastIndexOfChar ('/');

        const auto folder = lastSlash >= 0
                ? path.substring (0, lastSlash)
                : juce::String();

        if (folder.equalsIgnoreCase (cleanFolder))
        {
            names.add (audioProcessor.getPresetName (i));
            presetGridTargetIndices.add (i);
            presetGridTargetFolders.add (juce::String());
        }
    }

    auto presetDirectory = juce::File::getSpecialLocation (
            juce::File::userDocumentsDirectory)
            .getChildFile ("StarDonk")
            .getChildFile ("Presets");

    if (cleanFolder.isNotEmpty())
        presetDirectory = presetDirectory.getChildFile (cleanFolder);

    const auto directories = presetDirectory.findChildFiles (
            juce::File::findDirectories,
            false);

    for (const auto& directory : directories)
    {
        const auto leaf = directory.getFileName();

        if (leaf.isEmpty())
            continue;

        const auto fullFolder = cleanFolder.isEmpty()
                ? leaf
                : cleanFolder + "/" + leaf;

        childFolders.add (fullFolder);
    }

    childFolders.sort (true);

    for (const auto& folder : childFolders)
    {
        const int slash = folder.lastIndexOfChar ('/');
        const auto leaf = slash >= 0 ? folder.substring (slash + 1) : folder;
        names.add ("FOLDER: " + leaf);
        presetGridTargetIndices.add (-1);
        presetGridTargetFolders.add (folder);
    }

    if (childFolders.size() > 0)
    {
        juce::StringArray finalNames;
        juce::Array<int> finalIndices;
        juce::StringArray finalFolders;
        int existingStart = cleanFolder.isNotEmpty() ? 1 : 0;

        if (cleanFolder.isNotEmpty())
        {
            finalNames.add (names[0]);
            finalIndices.add (presetGridTargetIndices[0]);
            finalFolders.add (presetGridTargetFolders[0]);
        }

        const int directPresetCount = names.size() - existingStart - childFolders.size();
        const int folderStart = existingStart + directPresetCount;

        for (int i = folderStart; i < names.size(); ++i)
        {
            finalNames.add (names[i]);
            finalIndices.add (presetGridTargetIndices[i]);
            finalFolders.add (presetGridTargetFolders[i]);
        }

        for (int i = existingStart; i < folderStart; ++i)
        {
            finalNames.add (names[i]);
            finalIndices.add (presetGridTargetIndices[i]);
            finalFolders.add (presetGridTargetFolders[i]);
        }

        names = finalNames;
        presetGridTargetIndices = finalIndices;
        presetGridTargetFolders = finalFolders;
    }

    presetGrid->setPresets (names);
    const int gridWidth = presetGrid->getPreferredWidth();
    const int gridHeight = presetGrid->getPreferredHeight();
    const auto presetBounds = presetBox.getBounds();
    int x = presetBounds.getX() + presetBounds.getWidth() / 2 - gridWidth / 2;
    int y = presetBounds.getY() - gridHeight - 8;
    x = juce::jlimit (5, getWidth() - gridWidth - 5, x);

    if (y < 5)
    {
        y = presetBounds.getBottom() + 8;

        if (y + gridHeight > getHeight() - 5)
            y = getHeight() - gridHeight - 5;
    }

    presetGrid->setBounds (x, y, gridWidth, gridHeight);
    presetGrid->toFront (true);
    presetGrid->setVisible (true);
    presetGrid->repaint();
}

void NewProjectAudioProcessorEditor::
hidePresetGrid()
{
    if (presetGrid != nullptr)
        presetGrid->setVisible (false);
}

void NewProjectAudioProcessorEditor::showHelpPanel()
{
    hidePresetGrid();

    if (helpPanel == nullptr)
    {
        helpPanel = std::make_unique<StarDonkHelpComponent>();

        helpPanel->onClose = [this]()
            {
                hideHelpPanel();
            };

        addAndMakeVisible (*helpPanel);
    }

    helpPanel->setBounds (getLocalBounds());
    helpPanel->setVisible (true);
    helpPanel->toFront (true);
}

void NewProjectAudioProcessorEditor::hideHelpPanel()
{
    if (helpPanel != nullptr)
        helpPanel->setVisible (false);

}

void NewProjectAudioProcessorEditor::
previousPreset()
{
    hidePresetGrid();
    const int count = audioProcessor.getNumPresets();

    if (count <= 0)
        return;

    int index = audioProcessor.getCurrentPresetIndex();

    if (index < 0)
        index = 0;

    --index;

    if (index < 0)
        index = count - 1;

    audioProcessor.loadPreset (index);
    updateSlidersFromProcessor();
    refreshPresetBox();
    triggerFlyingPresetName();
}

void NewProjectAudioProcessorEditor::
nextPreset()
{
    hidePresetGrid();
    const int count = audioProcessor.getNumPresets();

    if (count <= 0)
        return;

    int index = audioProcessor.getCurrentPresetIndex();

    if (index < 0)
        index = 0;

    ++index;

    if (index >= count)
        index = 0;

    audioProcessor.loadPreset (index);
    updateSlidersFromProcessor();
    refreshPresetBox();
    triggerFlyingPresetName();
}

void NewProjectAudioProcessorEditor::
savePreset()
{
    savePresetWithSuggestedName ("My Donk");
}

juce::String NewProjectAudioProcessorEditor::makeRandomPresetName()
{
    static const char* firstWords[] =
    {
        // slavisk mat o dricka
        "Pierogi", "Borscht", "Pelmeni", "Pirozhki", "Blini", "Syrniki",
        "Kielbasa", "Bigos", "Sarma", "Burek", "Ajvar", "Banitsa",
        "Shopska", "Cevapi", "Pljeskavica", "Pampushki", "Vareniki",
        "Golubtsi", "Draniki", "Kasha", "Medovik", "Kvass", "Kompot",
        "Pickle", "Semechki", "Sunflower", "Dumpling",

        // bilar o maskiner typ
        "Lada", "Yugo", "Zastava", "Skoda", "Tatra", "ZAZ", "Moskvich",
        "Volga", "Niva", "Samara", "Trabant", "Polonez", "Maluch",
        "Tractor", "Moped", "Wagon", "Hatchback",

        // gopnik o hardbass meme grejs
        "Gopnik", "Hardbass", "Tracksuit", "Squat", "Bench", "Basement",
        "Subwoofer", "Goldchain", "Slippers", "Carpet", "Kettle",
        "Parkinglot", "Concrete", "BoomBox", "Bassline", "Turbo",
        "Rubber", "Speaker", "Neighbor", "Adidaska", "Bucket", "Garage",
        "Stairwell", "Courtyard", "Radiator", "Teapot", "Picklejar",
        "Sunflowerbag", "Donkmobile", "Blyatiful", "Babushka",
        "Balaclava", "PlasticChair", "GarageDoor", "ConcreteBench",
        "SunflowerBoss", "KvassDealer", "Squatmaster"
    };

    constexpr int wordCount = static_cast<int> (
            sizeof (firstWords)
            / sizeof (firstWords[0]));

    return juce::String (
        firstWords[funRandom.nextInt (wordCount)])
        + " Donk";
}

void NewProjectAudioProcessorEditor::savePresetWithSuggestedName (const juce::String& suggestedName)
{
    hidePresetGrid();

    auto* alertWindow = new juce::AlertWindow ("SAVE STAR DONK", currentPresetFolder.isEmpty()
            ? "Enter a name for this .donk preset:"
            : "Saving inside: " + currentPresetFolder,
        juce::MessageBoxIconType::NoIcon);

    alertWindow->addTextEditor ("presetName", suggestedName, "NAME:");
    alertWindow->addButton ("RANDOM NAME", 2);
    alertWindow->addButton ("SAVE", 1, juce::KeyPress (juce::KeyPress::returnKey));
    alertWindow->addButton ("CANCEL", 0, juce::KeyPress (juce::KeyPress::escapeKey));

    alertWindow->enterModalState (true, juce::ModalCallbackFunction::create ([this, alertWindow] (int result)
            {
                const auto name = alertWindow->getTextEditorContents ("presetName");
                delete alertWindow;

                if (result == 2)
                {
                    savePresetWithSuggestedName (makeRandomPresetName());
                    return;
                }

                if (result == 1)
                {
                    if (audioProcessor.presetNameExists (
                            name,
                            currentPresetFolder))
                    {
                        juce::AlertWindow::showMessageBoxAsync (
                            juce::MessageBoxIconType::WarningIcon,
                            "BLYAT",
                            "Blyat, you already have this name saved",
                            "BLYAT");

                        return;
                    }

                    audioProcessor.saveCurrentAsPreset (name, currentPresetFolder);
                    refreshPresetBox();
                    updateSlidersFromProcessor();
                    triggerFlyingPresetName();
                }

            }),
        false);
}

NewProjectAudioProcessorEditor::DonkSnapshot
NewProjectAudioProcessorEditor::captureDonkSnapshot() const
{
    DonkSnapshot s;
    s.pitchDrop = audioProcessor.getPitchDrop();
    s.decay = audioProcessor.getDecay();
    s.knock = audioProcessor.getKnock();
    s.ratio = audioProcessor.getRatio();
    s.shape = audioProcessor.getShape();
    s.tone = audioProcessor.getTone();
    s.body = audioProcessor.getBody();
    s.drive = audioProcessor.getDrive();
    s.experimentalControl = audioProcessor.getExperimentalControlEnabled();
    s.experimentalRandom = audioProcessor.getExperimentalRandomEnabled();

    for (int i = 0;
         i < NewProjectAudioProcessor::AdvancedParameterCount;
         ++i)
    {
        s.advanced[static_cast<size_t> (i)] = audioProcessor.getAdvancedParameterValue (i);
    }

    return s;
}

void NewProjectAudioProcessorEditor::restoreDonkSnapshot (const DonkSnapshot& s)
{
    audioProcessor.setExperimentalControlEnabled (s.experimentalControl);
    audioProcessor.setExperimentalRandomEnabled (s.experimentalRandom);
    audioProcessor.setPitchDrop (s.pitchDrop);
    audioProcessor.setDecay (s.decay);
    audioProcessor.setKnock (s.knock);
    audioProcessor.setRatio (s.ratio);
    audioProcessor.setShape (s.shape);
    audioProcessor.setTone (s.tone);
    audioProcessor.setBody (s.body);
    audioProcessor.setDrive (s.drive);

    for (int i = 0;
         i < NewProjectAudioProcessor::AdvancedParameterCount;
         ++i)
    {
        audioProcessor.setAdvancedParameterValue (
            i,
            s.advanced[static_cast<size_t> (i)]);
    }

    updateSlidersFromProcessor();
}

void NewProjectAudioProcessorEditor::giveBackDonk()
{
    if (! hasPreviousRandom)
        return;

    restoreDonkSnapshot (previousRandomSnapshot);
    hasPreviousRandom = false;
    refreshPresetBox();
    repaint();
}

void NewProjectAudioProcessorEditor::updateExperimentalFunVisibility()
{
    const bool on = experimentalFunToggle.getToggleState();
    funPanel.setVisible (on);

    if (on)
    {
        funPanel.toFront (true);
        updateMemeVisibility();
    }
}

void NewProjectAudioProcessorEditor::updateMemeVisibility()
{
    const bool show = experimentalFunToggle.getToggleState() && memePanelOpen;
    bassEarthquakeToggle.setVisible (show);
    screenShakeToggle.setVisible (show);
    faceSquashToggle.setVisible (show);
    ladaModeToggle.setVisible (show);
    discoModeToggle.setVisible (show);
    flyingPresetToggle.setVisible (show);
    memeButton.setButtonText (show ? "MEME  -" : "MEME  +");
    funPanel.repaint();
}

void NewProjectAudioProcessorEditor::restoreBassEarthquake()
{
    if (earthquakeHasSnapshot)
        restoreDonkSnapshot (earthquakeSnapshot);

    earthquakeHasSnapshot = false;
    earthquakeReturnFrames = 0;
}

void NewProjectAudioProcessorEditor::disableAllExperimentalFun()
{
    stopDoNotPress (true);
    restoreBassEarthquake();
    semechkiRainToggle.setToggleState (false, juce::dontSendNotification);
    babushkaSafeToggle.setToggleState (false, juce::dontSendNotification);
    neighborMeterToggle.setToggleState (false, juce::dontSendNotification);
    redAlertToggle.setToggleState (false, juce::dontSendNotification);
    impactCracksToggle.setToggleState (false, juce::dontSendNotification);
    bassEarthquakeToggle.setToggleState (false, juce::dontSendNotification);
    screenShakeToggle.setToggleState (false, juce::dontSendNotification);
    faceSquashToggle.setToggleState (false, juce::dontSendNotification);
    ladaModeToggle.setToggleState (false, juce::dontSendNotification);
    discoModeToggle.setToggleState (false, juce::dontSendNotification);
    flyingPresetToggle.setToggleState (false, juce::dontSendNotification);
    audioProcessor.setBabushkaSafeMode (false);
    audioProcessor.setFunDonkLoopEnabled (false);
    memePanelOpen = false;
    discoLife = 0.0f;
    faceSquashLife = 0.0f;
    wideBoiWidth = 1.0f;
    wideBoiHeight = 1.0f;
    wideBoiTargetWidth = 1.0f;
    wideBoiTargetHeight = 1.0f;
    wideBoiPhase = 0.0f;
    flyingPresetSpawnFrames = 0;
    impactCracks.clear();
    flyingPresets.clear();
    funParticles.clear();
    uiShakeX = 0.0f;
    uiShakeY = 0.0f;
    updateMemeVisibility();
    applyChildShakeTransform();
}

void NewProjectAudioProcessorEditor::triggerRandomExplosion()
{
    const auto c = randomButton.getBounds().getCentre().toFloat();

    for (int i = 0; i < 30; ++i)
    {
        FunParticle p;
        p.x = c.x;
        p.y = c.y;
        const float angle = funRandom.nextFloat() * juce::MathConstants<float>::twoPi;
        const float speed = 1.5f + funRandom.nextFloat() * 5.0f;
        p.vx = std::cos (angle) * speed;
        p.vy = std::sin (angle) * speed - 1.5f;
        p.life = 1.0f;
        p.text = (i % 5 == 0);
        funParticles.add (p);
    }
}

void NewProjectAudioProcessorEditor::startDoNotPress()
{
    if (doNotPressRunning)
        return;

    doNotPressSnapshot = captureDonkSnapshot();
    doNotPressRunning = true;
    doNotPressFrames = 180; // typ 3 sekunder om de kör 60 hz
    doNotPressRandomFrames = 0;
    audioProcessor.setFunDonkLoopEnabled (true);
    doNotPressButton.setButtonText ("BLYAT!!!");
    triggerRandomExplosion();
}

void NewProjectAudioProcessorEditor::stopDoNotPress (bool restoreSound)
{
    if (! doNotPressRunning)
    {
        audioProcessor.setFunDonkLoopEnabled (false);
        doNotPressButton.setButtonText ("DO NOT PRESS");
        return;
    }

    doNotPressRunning = false;
    doNotPressFrames = 0;
    doNotPressRandomFrames = 0;
    audioProcessor.setFunDonkLoopEnabled (false);
    doNotPressButton.setButtonText ("DO NOT PRESS");

    if (restoreSound)
        restoreDonkSnapshot (doNotPressSnapshot);
}

void NewProjectAudioProcessorEditor::triggerBassEarthquake()
{
    if (! bassEarthquakeToggle.getToggleState())
        return;

    if (! earthquakeHasSnapshot)
    {
        earthquakeSnapshot = captureDonkSnapshot();
        earthquakeHasSnapshot = true;
    }

    audioProcessor.setPitchDrop (earthquakeSnapshot.pitchDrop + funRandom.nextFloat() * 80.0f - 40.0f);
    audioProcessor.setDecay (earthquakeSnapshot.decay + funRandom.nextFloat() * 2.0f - 0.6f);
    audioProcessor.setKnock (earthquakeSnapshot.knock + funRandom.nextFloat() * 20.0f - 10.0f);
    audioProcessor.setRatio (earthquakeSnapshot.ratio + funRandom.nextFloat() * 12.0f - 6.0f);
    audioProcessor.setShape (earthquakeSnapshot.shape + funRandom.nextFloat() * 2.0f - 1.0f);
    audioProcessor.setTone (earthquakeSnapshot.tone + funRandom.nextFloat() * 1.2f - 0.6f);
    audioProcessor.setBody (earthquakeSnapshot.body + funRandom.nextFloat() * 1.4f - 0.7f);
    audioProcessor.setDrive (earthquakeSnapshot.drive + funRandom.nextFloat() * 18.0f - 7.0f);

    if (audioProcessor.getAdvancedModeEnabled())
    {
        for (int i = 0;
             i < NewProjectAudioProcessor::AdvancedParameterCount;
             ++i)
        {
            const bool legacyClassic = i >= NewProjectAudioProcessor::ClassicBlend
                && i <= NewProjectAudioProcessor::ClassicELevel;

            if (legacyClassic)
                continue;

            const float minimum = audioProcessor.getAdvancedParameterMinimum (i);
            const float maximum = audioProcessor.getAdvancedParameterMaximum (i);
            const float span = maximum - minimum;

            const float original = earthquakeSnapshot.advanced[
                    static_cast<size_t> (i)];

            const float totallyRandom = minimum
                + funRandom.nextFloat() * span;

            const float violence = 0.58f
                + funRandom.nextFloat() * 0.38f;

            float shaken = original
                + (totallyRandom - original)
                * violence;

            // ibland får nån hit en eller två rattar typ nästan helt åt kanten
            if (funRandom.nextFloat() < 0.12f)
            {
                shaken = funRandom.nextBool()
                        ? minimum + span * 0.04f
                        : maximum - span * 0.04f;
            }

            audioProcessor.setAdvancedParameterValue (
                i,
                shaken);
        }
    }

    earthquakeReturnFrames = 10;
    updateSlidersFromProcessor();
}

void NewProjectAudioProcessorEditor::triggerFlyingPresetName()
{
    if (! flyingPresetToggle.getToggleState())
        return;

    FlyingPreset flyer;
    const int presetCount = audioProcessor.getNumPresets();
    if (presetCount > 0)
    {
        const int index = funRandom.nextInt (presetCount);
        flyer.text = audioProcessor.getPresetName (index);
    }

    if (flyer.text.isEmpty())
        flyer.text = makeRandomPresetName();

    const int side = funRandom.nextInt (4);
    flyer.fontSize = 18.0f + funRandom.nextFloat() * 25.0f;
    flyer.angle = (funRandom.nextFloat() * 2.0f - 1.0f) * 0.32f;
    flyer.life = 1.0f;

    if (side == 0)
    {
        flyer.x = -340.0f;
        flyer.y = 50.0f + funRandom.nextFloat() * (static_cast<float> (getHeight()) - 100.0f);
        flyer.vx = 7.0f + funRandom.nextFloat() * 7.0f;
        flyer.vy = (funRandom.nextFloat() * 2.0f - 1.0f) * 2.5f;
    }
    else if (side == 1)
    {
        flyer.x = static_cast<float> (getWidth()) + 20.0f;
        flyer.y = 50.0f + funRandom.nextFloat() * (static_cast<float> (getHeight()) - 100.0f);
        flyer.vx = -(7.0f + funRandom.nextFloat() * 7.0f);
        flyer.vy = (funRandom.nextFloat() * 2.0f - 1.0f) * 2.5f;
    }
    else if (side == 2)
    {
        flyer.x = funRandom.nextFloat() * static_cast<float> (getWidth());
        flyer.y = -55.0f;
        flyer.vx = (funRandom.nextFloat() * 2.0f - 1.0f) * 4.0f;
        flyer.vy = 6.0f + funRandom.nextFloat() * 6.0f;
    }
    else
    {
        flyer.x = funRandom.nextFloat() * static_cast<float> (getWidth());
        flyer.y = static_cast<float> (getHeight()) + 20.0f;
        flyer.vx = (funRandom.nextFloat() * 2.0f - 1.0f) * 4.0f;
        flyer.vy = -(6.0f + funRandom.nextFloat() * 6.0f);
    }

    flyingPresets.add (flyer);
}

void NewProjectAudioProcessorEditor::spawnImpactCrack()
{
    ImpactCrack crack;
    crack.x = 45.0f + funRandom.nextFloat() * (static_cast<float> (getWidth()) - 90.0f);
    crack.y = 55.0f + funRandom.nextFloat() * (static_cast<float> (getHeight()) - 110.0f);
    crack.size = 45.0f + funRandom.nextFloat() * 185.0f;
    crack.rotation = funRandom.nextFloat() * juce::MathConstants<float>::twoPi;
    crack.life = 1.0f;
    crack.branches = 5 + funRandom.nextInt (8);
    crack.jitter = funRandom.nextFloat() * 1000.0f;
    impactCracks.add (crack);
}

void NewProjectAudioProcessorEditor::closeExperimentalFunPanel()
{
    funPanel.setVisible (false);

    juce::AlertWindow::showMessageBoxAsync (
        juce::MessageBoxIconType::WarningIcon,
        "EXPERIMENTAL FUN",
        "You still have experimental fun on, no guarantees what happens next :-D",
        "BLYAT");
}

void NewProjectAudioProcessorEditor::applyChildShakeTransform()
{
    const auto transform = juce::AffineTransform::translation (uiShakeX, uiShakeY);

    for (int i = 0; i < getNumChildComponents(); ++i)
    {
        auto* child = getChildComponent (i);
        if (child != nullptr && child != &funPanel && child != helpPanel.get() && child != presetGrid.get())
            child->setTransform (transform);
    }
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    updateComputerKeyboardState();

    if (! pitchDropSlider.isMouseButtonDown())
        pitchDropSlider.setValue (audioProcessor.getPitchDrop(), juce::dontSendNotification);

    if (! decaySlider.isMouseButtonDown())
        decaySlider.setValue (audioProcessor.getDecay(), juce::dontSendNotification);

    if (! knockSlider.isMouseButtonDown())
        knockSlider.setValue (audioProcessor.getKnock(), juce::dontSendNotification);

    if (! ratioSlider.isMouseButtonDown())
        ratioSlider.setValue (audioProcessor.getRatio(), juce::dontSendNotification);

    if (! shapeSlider.isMouseButtonDown())
        shapeSlider.setValue (audioProcessor.getShape(), juce::dontSendNotification);

    if (! toneSlider.isMouseButtonDown())
        toneSlider.setValue (audioProcessor.getTone(), juce::dontSendNotification);

    if (! bodySlider.isMouseButtonDown())
        bodySlider.setValue (audioProcessor.getBody(), juce::dontSendNotification);

    if (! driveSlider.isMouseButtonDown())
        driveSlider.setValue (audioProcessor.getDrive(), juce::dontSendNotification);

    if (! reverbSlider.isMouseButtonDown())
        reverbSlider.setValue (audioProcessor.getReverbMix() * 100.0f, juce::dontSendNotification);

    if (! sidechainAmountSlider.isMouseButtonDown())
        sidechainAmountSlider.setValue (audioProcessor.getSidechainAmount() * 100.0f, juce::dontSendNotification);

    if (! sidechainPositionSlider.isMouseButtonDown())
        sidechainPositionSlider.setValue (audioProcessor.getSidechainPosition() * 100.0f, juce::dontSendNotification);

    if (! declickerStrengthSlider.isMouseButtonDown())
        declickerStrengthSlider.setValue (audioProcessor.getDeclickerStrength() * 100.0f, juce::dontSendNotification);

    if (! volumeSlider.isMouseButtonDown())
        volumeSlider.setValue (audioProcessor.getVolume() * 100.0f, juce::dontSendNotification);

    if (! donkLoopSpeedSlider.isMouseButtonDown())
        donkLoopSpeedSlider.setValue (
            audioProcessor.getDonkLoopSpeed(),
            juce::dontSendNotification);

    basslineToggle.setToggleState (
        audioProcessor.getBasslineEnabled(),
        juce::dontSendNotification);

    reverbToggle.setToggleState (
        audioProcessor.getReverbEnabled(),
        juce::dontSendNotification);

    sidechainToggle.setToggleState (
        audioProcessor.getSidechainEnabled(),
        juce::dontSendNotification);

    sidechainAfterReverbToggle.setToggleState (
        audioProcessor.getSidechainAfterReverb(),
        juce::dontSendNotification);

    const bool showReverbNow = audioProcessor.getReverbEnabled();
    reverbLabel.setVisible (showReverbNow);
    reverbSlider.setVisible (showReverbNow);
    const bool showSidechainNow = audioProcessor.getSidechainEnabled();
    sidechainAmountLabel.setVisible (showSidechainNow);
    sidechainAmountSlider.setVisible (showSidechainNow);
    sidechainPositionLabel.setVisible (showSidechainNow);
    sidechainPositionSlider.setVisible (showSidechainNow);

    sidechainAfterReverbToggle.setVisible (
        showSidechainNow
        && showReverbNow);

    declickerToggle.setToggleState (
        audioProcessor.getDeclickerEnabled(),
        juce::dontSendNotification);

    const bool showDeclickerStrengthNow = audioProcessor.getDeclickerEnabled();
    declickerStrengthLabel.setVisible (showDeclickerStrengthNow);
    declickerStrengthSlider.setVisible (showDeclickerStrengthNow);

    animationsOffToggle.setToggleState (
        audioProcessor.getAnimationsDisabled(),
        juce::dontSendNotification);

    minimalisticToggle.setToggleState (
        audioProcessor.getMinimalisticEnabled(),
        juce::dontSendNotification);

    advancedModeToggle.setToggleState (
        audioProcessor.getAdvancedModeEnabled(),
        juce::dontSendNotification);

    if (audioProcessor.getAdvancedModeEnabled())
        updateAdvancedControlsFromProcessor();

    const bool animationsDisabled = audioProcessor.getAnimationsDisabled()
        || audioProcessor.getMinimalisticEnabled();

    if (animationsDisabled)
    {
        starEnergy = 0.0f;
        faceBounce = 0.0f;
        lastNoteTrigger = audioProcessor.getNoteTriggerCount();
    }
    else
    {
        const float activity = audioProcessor.getDonkActivity();

        starEnergy = starEnergy * 0.75f
            +
            activity * 0.25f;

        const uint64_t triggerCount = audioProcessor.getNoteTriggerCount();

        if (triggerCount != lastNoteTrigger)
        {
            lastNoteTrigger = triggerCount;
            faceBounce = 1.0f;
        }

        faceBounce *= 0.78f;

        if (faceBounce < 0.001f)
            faceBounce = 0.0f;

        for (int i = 0;
             i < 45;
             ++i)
        {
            starY[i] += starSpeed[i]
                *
                (1.0f +
                 starEnergy * 12.0f);

            if (starEnergy > 0.08f)
            {
                starX[i] += (starX[i] - 0.5f)
                    *
                    starEnergy
                    *
                    0.10f;
            }

            if (starY[i] > 1.0f)
            {
                starY[i] = 0.0f;

                starX[i] = juce::Random::getSystemRandom()
                        .nextFloat();
            }

            if (starX[i] < 0.0f)
                starX[i] = 1.0f;

            if (starX[i] > 1.0f)
                starX[i] = 0.0f;
        }
    }

    const uint64_t currentTrigger = audioProcessor.getNoteTriggerCount();

    if (funSeenTrigger == 0)
        funSeenTrigger = currentTrigger;

    if (currentTrigger != funSeenTrigger)
    {
        funSeenTrigger = currentTrigger;

        if (impactCracksToggle.getToggleState())
            spawnImpactCrack();

        if (faceSquashToggle.getToggleState())
        {
            faceSquashLife = 1.0f;
            wideBoiPhase = 0.0f;
            const float roll = funRandom.nextFloat();

            if (roll < 0.18f)
            {
                // sällsynt mega boi, ibland tar han typ halva pluginet lol
                wideBoiTargetWidth = 5.2f
                    + funRandom.nextFloat() * 2.4f;

                wideBoiTargetHeight = 3.4f
                    + funRandom.nextFloat() * 3.0f;
            }
            else
            {
                wideBoiTargetWidth = 2.4f
                    + funRandom.nextFloat() * 2.9f;

                if (funRandom.nextFloat() < 0.30f)
                {
                    wideBoiTargetHeight = 1.8f
                        + funRandom.nextFloat() * 2.7f;
                }
                else
                {
                    wideBoiTargetHeight = 0.62f
                        + funRandom.nextFloat() * 0.72f;
                }
            }
        }

        if (bassEarthquakeToggle.getToggleState())
            triggerBassEarthquake();
    }

    if (babushkaSafeToggle.getToggleState() != audioProcessor.getBabushkaSafeMode())
        audioProcessor.setBabushkaSafeMode (babushkaSafeToggle.getToggleState());

    if (doNotPressRunning)
    {
        --doNotPressFrames;
        --doNotPressRandomFrames;

        if (doNotPressRandomFrames <= 0)
        {
            doNotPressRandomFrames = 18; // typ 0.3 sek vid 60 hz
            audioProcessor.setExperimentalControlEnabled (true);
            audioProcessor.setExperimentalRandomEnabled (true);
            audioProcessor.randomizeSound();
            updateSlidersFromProcessor();
            triggerRandomExplosion();
            discoLife = 1.0f;
            discoHue = funRandom.nextFloat();
        }

        if (doNotPressFrames <= 0)
            stopDoNotPress (true);
    }

    if (earthquakeReturnFrames > 0)
    {
        --earthquakeReturnFrames;
        if (earthquakeReturnFrames == 0 && earthquakeHasSnapshot)
            restoreDonkSnapshot (earthquakeSnapshot);
    }

    discoLife *= 0.94f;

    if (faceSquashToggle.getToggleState())
    {
        // mycke långsammare än gamla kaos squash stretch grejen
        wideBoiPhase += 0.035f;

        if (faceSquashLife > 0.001f)
        {
            faceSquashLife *= 0.982f;

            const float desiredWidth = 1.0f
                + (wideBoiTargetWidth - 1.0f)
                * faceSquashLife;

            const float desiredHeight = 1.0f
                + (wideBoiTargetHeight - 1.0f)
                * faceSquashLife;

            wideBoiWidth += (desiredWidth - wideBoiWidth)
                * 0.075f;

            wideBoiHeight += (desiredHeight - wideBoiHeight)
                * 0.075f;
        }
        else
        {
            faceSquashLife = 0.0f;

            wideBoiWidth += (1.0f - wideBoiWidth)
                * 0.055f;

            wideBoiHeight += (1.0f - wideBoiHeight)
                * 0.055f;
        }
    }
    else
    {
        faceSquashLife = 0.0f;
        wideBoiWidth = 1.0f;
        wideBoiHeight = 1.0f;
        wideBoiTargetWidth = 1.0f;
        wideBoiTargetHeight = 1.0f;
    }

    for (int i = impactCracks.size(); --i >= 0;)
    {
        auto& crack = impactCracks.getReference (i);
        crack.life *= 0.955f;
        if (crack.life < 0.015f)
            impactCracks.remove (i);
    }

    if (discoModeToggle.getToggleState())
    {
        discoHue = std::fmod (discoHue + 0.013f, 1.0f);
        discoLife = 1.0f;
    }

    if (flyingPresetToggle.getToggleState())
    {
        --flyingPresetSpawnFrames;
        if (flyingPresetSpawnFrames <= 0)
        {
            triggerFlyingPresetName();
            flyingPresetSpawnFrames = 10 + funRandom.nextInt (20);
        }
    }
    else
    {
        flyingPresetSpawnFrames = 0;
        flyingPresets.clear();
    }

    for (int i = flyingPresets.size(); --i >= 0;)
    {
        auto& flyer = flyingPresets.getReference (i);
        flyer.x += flyer.vx;
        flyer.y += flyer.vy;
        flyer.life -= 0.012f;
        flyer.angle += 0.006f * (flyer.vx >= 0.0f ? 1.0f : -1.0f);

        if (flyer.life <= 0.0f
            || flyer.x < -500.0f || flyer.x > static_cast<float> (getWidth()) + 500.0f
            || flyer.y < -250.0f || flyer.y > static_cast<float> (getHeight()) + 250.0f)
        {
            flyingPresets.remove (i);
        }
    }

    for (int i = funParticles.size(); --i >= 0;)
    {
        auto& p = funParticles.getReference (i);
        p.x += p.vx;
        p.y += p.vy;
        p.vy += 0.10f;
        p.life -= 0.025f;
        if (p.life <= 0.0f)
            funParticles.remove (i);
    }

    if (experimentalFunToggle.getToggleState()
        && (screenShakeToggle.getToggleState() || ladaModeToggle.getToggleState()))
    {
        float strength = 0.0f;

        if (screenShakeToggle.getToggleState() && starEnergy > 0.05f)
            strength = 1.0f + starEnergy * 6.0f;

        if (ladaModeToggle.getToggleState())
            strength += 2.0f + funRandom.nextFloat() * 3.0f;

        uiShakeX = (funRandom.nextFloat() * 2.0f - 1.0f) * strength;
        uiShakeY = (funRandom.nextFloat() * 2.0f - 1.0f) * strength;
    }
    else
    {
        uiShakeX = 0.0f;
        uiShakeY = 0.0f;
    }

    applyChildShakeTransform();
    repaint();
}

void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    if (audioProcessor.getMinimalisticEnabled())
    {
        g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::white);
        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (22.0f)
                    .withStyle ("Bold")));

        g.drawText (
            "STAR DONK",
            0,
            18,
            getWidth(),
            32,
            juce::Justification::centred);

        g.setColour (juce::Colour (0xff777777));
        g.drawRoundedRectangle (
            20.0f,
            260.0f,
            static_cast<float> (getWidth() - 40),
            audioProcessor.getAdvancedModeEnabled()
                ? static_cast<float> (getHeight() - 305)
                : 330.0f,
            12.0f,
            1.0f);

        // behåll stardonk grejen längst ner även i minimal mode
        g.setColour (juce::Colour (0xff777777));
        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (10.0f)));

        g.drawText (
            "100% Free - Made By STARSLAV - SHARE IT - SEED IT - LEECH IT",
            0,
            getHeight() - 16,
            getWidth(),
            15,
            juce::Justification::centred);

        if (audioProcessor.getAdvancedModeEnabled())
        {
            g.setFont (juce::Font (juce::FontOptions()
                        .withName ("Arial")
                        .withHeight (11.0f)
                        .withStyle ("Bold")));

            g.setColour (juce::Colours::white);
            g.drawText (
                "ADVANCED ENGINE",
                30,
                264,
                getWidth() - 60,
                18,
                juce::Justification::centred);

            return;
        }

        const int knobWidth = 135;
        const int spacing = 12;
        const int totalWidth = knobWidth * 4 + spacing * 3;
        const int startX = (getWidth() - totalWidth) / 2;
        const int row1Y = 282;
        const int row2Y = 406;

        const auto drawVerticalLabel = [&g] (int x, int y, const juce::String& label)
            {
                juce::String vertical;

                for (int i = 0; i < label.length(); ++i)
                {
                    vertical << juce::String::charToString (label[i]);

                    if (i < label.length() - 1)
                        vertical << "\n";
                }

                g.setColour (juce::Colours::white);
                g.setFont (juce::Font (juce::FontOptions()
                            .withName ("Arial")
                            .withHeight (10.0f)
                            .withStyle ("Bold")));

                g.drawFittedText (
                    vertical,
                    juce::Rectangle<int> (x, y, 18, 92),
                    juce::Justification::centred,
                    12);
            };

        drawVerticalLabel (startX - 18, row1Y + 12, "PITCH");
        drawVerticalLabel (startX + (knobWidth + spacing) - 18, row1Y + 12, "DECAY");
        drawVerticalLabel (startX + (knobWidth + spacing) * 2 - 18, row1Y + 12, "KNOCK");
        drawVerticalLabel (startX + (knobWidth + spacing) * 3 - 18, row1Y + 12, "RATIO");
        drawVerticalLabel (startX - 18, row2Y + 12, "SHAPE");
        drawVerticalLabel (startX + (knobWidth + spacing) - 18, row2Y + 12, "TONE");
        drawVerticalLabel (startX + (knobWidth + spacing) * 2 - 18, row2Y + 12, "BODY");
        drawVerticalLabel (startX + (knobWidth + spacing) * 3 - 18, row2Y + 12, "DRIVE");
        return;
    }
    g.fillAll (juce::Colour (0xff08070d));
    g.addTransform (juce::AffineTransform::translation (uiShakeX, uiShakeY));
    juce::Colour topBackground (0xff19132a);

    if (discoModeToggle.getToggleState())
        topBackground = juce::Colour::fromHSV (discoHue, 0.95f, 0.72f, 1.0f);

    g.setGradientFill (
        juce::ColourGradient (
            topBackground,
            0.0f,
            0.0f,

            juce::Colour (
                0xff08070d),
            0.0f,
            610.0f,

            false));

    g.fillRect (getLocalBounds());

    if (discoModeToggle.getToggleState())
    {
        const float w = static_cast<float> (getWidth());
        const float h = static_cast<float> (getHeight());

        for (int band = 0; band < 9; ++band)
        {
            const float hue = std::fmod (discoHue + static_cast<float> (band) / 9.0f, 1.0f);
            g.setColour (juce::Colour::fromHSV (hue, 0.95f, 0.95f, 0.17f));
            g.fillRect (static_cast<float> (band) * w / 9.0f, 0.0f, w / 9.0f + 1.0f, h);
        }

        for (int spot = 0; spot < 6; ++spot)
        {
            const float phase = discoHue * juce::MathConstants<float>::twoPi + static_cast<float> (spot) * 1.17f;
            const float x = w * (0.5f + 0.46f * std::sin (phase * (1.0f + spot * 0.07f)));
            const float y = h * (0.5f + 0.43f * std::cos (phase * (1.2f + spot * 0.05f)));
            const float size = 80.0f + 55.0f * std::sin (phase * 1.7f + spot);
            const float hue = std::fmod (discoHue + static_cast<float> (spot) * 0.16f, 1.0f);
            g.setColour (juce::Colour::fromHSV (hue, 1.0f, 1.0f, 0.22f));
            g.fillEllipse (x - size * 0.5f, y - size * 0.5f, size, size);
        }
    }

    for (int i = 0;
         i < 45;
         ++i)
    {
        const float x = starX[i] *
            static_cast<float> (getWidth());

        const float y = starY[i] *
            static_cast<float> (getHeight());

        const float size = starSize[i]
            *
            (1.0f +
             starEnergy * 2.5f);

        const float alpha = juce::jlimit (
                0.0f,
                1.0f,
                0.35f +
                starEnergy * 0.65f);

        if (semechkiRainToggle.getToggleState() && semechkiImage.isValid())
        {
            const int seedW = juce::jmax (5, static_cast<int> (size * 4.5f));
            const int seedH = juce::jmax (8, static_cast<int> (size * 6.8f));
            g.setOpacity (alpha);
            g.drawImage (semechkiImage,
                         static_cast<int> (x), static_cast<int> (y), seedW, seedH,
                         0, 0, semechkiImage.getWidth(), semechkiImage.getHeight(), false);
            g.setOpacity (1.0f);
        }
        else
        {
            g.setColour (juce::Colour (0xffffdf66).withAlpha (alpha));

            if (starEnergy > 0.15f)
            {
                g.drawLine (x, y,
                            x - (starX[i] - 0.5f) * starEnergy * 80.0f,
                            y - starEnergy * 20.0f,
                            size);
            }
            else
            {
                g.fillEllipse (x, y, size, size);
            }
        }
    }

    const float centerX = static_cast<float> (
            getWidth())
        *
        0.5f;

    const float baseCenterY = 132.0f;

    const float glow = 75.0f
        +
        starEnergy * 55.0f
        +
        faceBounce * 15.0f;

    g.setColour (juce::Colour (0xffffc933)
            .withAlpha (
                juce::jlimit (
                    0.0f,
                    1.0f,
                    0.04f +
                    starEnergy * 0.12f)));

    g.fillEllipse (
        centerX - glow,
        baseCenterY - glow,
        glow * 2.0f,
        glow * 2.0f);

    g.setColour (
        juce::Colour (0xffffcf3f));

    g.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (38.0f)
                .withStyle ("Bold")));

    g.drawText (
        "STAR DONK",
        0,
        14,
        getWidth(),
        45,
        juce::Justification::centred);

    g.setColour (
        juce::Colour (0xffaaa4b7));

    g.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (12.0f)));

    g.drawText (
        "I GIVE YOU DONK - Dr Gopnik",
        0,
        57,
        getWidth(),
        20,
        juce::Justification::centred);

    const float bounceY = std::sin (
            faceBounce *
            juce::MathConstants<float>::pi)
        *
        18.0f;

    const float centerY = baseCenterY -
        bounceY;

    const float radius = 40.0f
        +
        starEnergy * 12.0f
        +
        faceBounce * 5.0f;

    const auto faceMode = audioProcessor.getFaceMode();
    const juce::Image* imageToDraw = nullptr;

    if (faceMode == NewProjectAudioProcessor::FaceMode::Custom
        &&
        customFaceImage.isValid())
    {
        imageToDraw = &customFaceImage;
    }
    else if (starSlavImage.isValid())
    {
        // starslav blir fallback också om custom filen bara försvinner
        imageToDraw = &starSlavImage;
    }

    if (imageToDraw != nullptr
        &&
        imageToDraw->isValid())
    {
        const int imageWidth = imageToDraw->getWidth();
        const int imageHeight = imageToDraw->getHeight();
        const float targetSize = radius * 2.0f;
        float widthScale = 1.0f;
        float heightScale = 1.0f;

        if (faceSquashToggle.getToggleState())
        {
            const float slowWobble = std::sin (wideBoiPhase)
                * 0.055f
                * faceSquashLife;

            widthScale = juce::jlimit (
                    0.50f,
                    8.0f,
                    wideBoiWidth
                    * (1.0f + slowWobble));

            heightScale = juce::jlimit (
                    0.45f,
                    6.8f,
                    wideBoiHeight
                    * (1.0f - slowWobble * 0.55f));
        }

        const float targetWidth = targetSize * widthScale;
        const float targetHeight = targetSize * heightScale;

        const float baseScale = juce::jmin (
                targetSize / static_cast<float> (juce::jmax (1, imageWidth)),
                targetSize / static_cast<float> (juce::jmax (1, imageHeight)));

        const int drawWidth = juce::jmax (
                1,
                static_cast<int> (
                    std::round (static_cast<float> (imageWidth)
                        * baseScale
                        * (targetWidth / targetSize))));

        const int drawHeight = juce::jmax (
                1,
                static_cast<int> (
                    std::round (static_cast<float> (imageHeight)
                        * baseScale
                        * (targetHeight / targetSize))));

        const int drawX = static_cast<int> (
                std::round (centerX - static_cast<float> (drawWidth)
                    *
                    0.5f));

        const int drawY = static_cast<int> (
                std::round (centerY - static_cast<float> (drawHeight)
                    *
                    0.5f));

        g.drawImage (
            *imageToDraw,
            drawX,
            drawY,
            drawWidth,
            drawHeight,
            0,
            0,
            imageWidth,
            imageHeight,
            false);
    }

    g.setColour (juce::Colours::white);

    g.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (18.0f)
                .withStyle ("Bold")));

    g.drawText (
        "D O N K",
        0,
        203,
        getWidth(),
        30,
        juce::Justification::centred);

    g.setColour (
        juce::Colour (0xff2c2638));

    g.fillRoundedRectangle (
        170.0f,
        242.0f,
        static_cast<float> (
            getWidth() - 340),
        3.0f,
        2.0f);

    g.setColour (
        juce::Colour (0xffffcc33));

    g.fillRoundedRectangle (170.0f, 242.0f, static_cast<float> (getWidth() - 340)
        *
        starEnergy,
        3.0f,
        2.0f);

    g.setColour (
        juce::Colour (0xff100d17));

    const float mainPanelHeight = audioProcessor.getAdvancedModeEnabled()
            ? static_cast<float> (getHeight() - 305)
            : 330.0f;

    g.fillRoundedRectangle (
        20.0f,
        260.0f,
        static_cast<float> (
            getWidth() - 40),
        mainPanelHeight,
        12.0f);

    g.setColour (
        juce::Colour (0xff30283b));

    g.drawRoundedRectangle (
        20.0f,
        260.0f,
        static_cast<float> (
            getWidth() - 40),
        mainPanelHeight,
        12.0f,
        1.0f);

    const int knobWidth = 135;
    const int spacing = 12;
    const int totalWidth = knobWidth * 4 + spacing * 3;
    const int startX = (getWidth() - totalWidth) / 2;
    const int row1Y = 282;
    const int row2Y = 406;

    const auto drawVerticalFunctionLabel = [&g] (int x, int y, const juce::String& text)
        {
            juce::String verticalText;

            for (int i = 0; i < text.length(); ++i)
            {
                const auto character = text[i];

                if (character == ' ')
                    continue;

                verticalText << juce::String::charToString (character);

                if (i < text.length() - 1)
                    verticalText << "\n";
            }

            g.setColour (
                juce::Colour (0xffff3a32));

            g.setFont (juce::Font (juce::FontOptions()
                        .withName ("Arial")
                        .withHeight (10.0f)
                        .withStyle ("Bold")));

            g.drawFittedText (
                verticalText,
                juce::Rectangle<int> (
                    x,
                    y,
                    18,
                    92),
                juce::Justification::centred,
                12);
        };

    if (! audioProcessor.getAdvancedModeEnabled())
    {
        drawVerticalFunctionLabel (startX - 18,                     row1Y + 12, "PITCH");
        drawVerticalFunctionLabel (startX + (knobWidth + spacing) - 18,         row1Y + 12, "DECAY");
        drawVerticalFunctionLabel (startX + (knobWidth + spacing) * 2 - 18,     row1Y + 12, "KNOCK");
        drawVerticalFunctionLabel (startX + (knobWidth + spacing) * 3 - 18,     row1Y + 12, "RATIO");
        drawVerticalFunctionLabel (startX - 18,                     row2Y + 12, "SHAPE");
        drawVerticalFunctionLabel (startX + (knobWidth + spacing) - 18,         row2Y + 12, "TONE");
        drawVerticalFunctionLabel (startX + (knobWidth + spacing) * 2 - 18,     row2Y + 12, "BODY");
        drawVerticalFunctionLabel (startX + (knobWidth + spacing) * 3 - 18,     row2Y + 12, "DRIVE");
    }
    else
    {
        g.setColour (juce::Colour (0xffffd34e));
        g.setFont (juce::Font (juce::FontOptions()
                    .withName ("Arial")
                    .withHeight (10.0f)
                    .withStyle ("Bold")));

        g.drawText (
            "ADVANCED ENGINE  //  CARRIER + FM1-4 + FILTER + ADSR",
            24,
            263,
            getWidth() - 48,
            16,
            juce::Justification::centred);
    }

    if (neighborMeterToggle.getToggleState())
    {
        const float rms = juce::jmax (0.000001f, audioProcessor.getOutputLevel());
        const float db = juce::Decibels::gainToDecibels (rms, -80.0f);
        const float complaint = juce::jlimit (0.0f, 1.0f, (db + 42.0f) / 38.0f);
        const char* label = complaint < 0.38f ? "CALM" : (complaint < 0.76f ? "ANGRY" : "POLICE");
        juce::Colour meterColour;
        if (complaint < 0.5f)
            meterColour = juce::Colours::green.interpolatedWith (juce::Colours::yellow, complaint * 2.0f);
        else
            meterColour = juce::Colours::yellow.interpolatedWith (juce::Colours::red, (complaint - 0.5f) * 2.0f);

        g.setColour (juce::Colour (0xff100d17));
        g.fillRoundedRectangle (510.0f, 214.0f, 185.0f, 34.0f, 6.0f);
        g.setColour (juce::Colour (0xff42384f));
        g.drawRoundedRectangle (510.0f, 214.0f, 185.0f, 34.0f, 6.0f, 1.0f);
        g.setColour (meterColour);
        g.fillRoundedRectangle (518.0f, 235.0f, 169.0f * complaint, 5.0f, 2.0f);
        g.setFont (juce::Font (juce::FontOptions().withName ("Arial").withHeight (10.0f).withStyle ("Bold")));
        g.drawText (juce::String ("NEIGHBORS: ") + label, 518, 218, 170, 15, juce::Justification::centredLeft);
    }

    if (redAlertToggle.getToggleState())
    {
        const bool alert = audioProcessor.getDrive() > 7.0f || audioProcessor.getOutputLevel() > 0.22f;

        if (alert && (juce::Time::getMillisecondCounter() / 120) % 2 == 0)
        {
            g.setColour (juce::Colours::red.withAlpha (0.18f));
            g.fillRect (getLocalBounds());
            g.setColour (juce::Colours::red);
            g.setFont (juce::Font (juce::FontOptions().withName ("Arial").withHeight (26.0f).withStyle ("Bold")));
            g.drawText ("TOO MUCH BLYAT", 0, 225, getWidth(), 36, juce::Justification::centred);
        }
    }

    if (impactCracksToggle.getToggleState())
    {
        for (const auto& crack : impactCracks)
        {
            g.setColour (juce::Colours::white.withAlpha (juce::jlimit (0.0f, 0.72f, crack.life * 0.72f)));

            for (int branch = 0; branch < crack.branches; ++branch)
            {
                const float baseAngle = crack.rotation
                    + (static_cast<float> (branch) / static_cast<float> (crack.branches)) * juce::MathConstants<float>::twoPi;
                const int segments = 3 + ((branch + static_cast<int> (crack.jitter)) % 4);
                float px = crack.x;
                float py = crack.y;

                for (int seg = 0; seg < segments; ++seg)
                {
                    const float f = static_cast<float> (seg + 1) / static_cast<float> (segments);
                    const float wobble = std::sin (crack.jitter * 0.013f + branch * 2.17f + seg * 1.91f) * 0.32f;
                    const float angle = baseAngle + wobble;
                    const float step = crack.size / static_cast<float> (segments) * (0.72f + 0.46f * std::abs (std::sin (crack.jitter + seg * 3.1f)));
                    const float nx = px + std::cos (angle) * step;
                    const float ny = py + std::sin (angle) * step;
                    g.drawLine (px, py, nx, ny, juce::jmax (0.5f, 1.35f * crack.life));

                    if (seg > 0 && ((seg + branch) % 2 == 0))
                    {
                        const float twigAngle = angle + ((branch % 2 == 0) ? 0.8f : -0.9f);
                        const float twig = step * (0.35f + 0.25f * f);
                        g.drawLine (nx, ny, nx + std::cos (twigAngle) * twig, ny + std::sin (twigAngle) * twig, 0.65f);
                    }

                    px = nx;
                    py = ny;
                }
            }
        }
    }

    for (const auto& p : funParticles)
    {
        g.setColour (juce::Colour (0xffffd34e).withAlpha (juce::jlimit (0.0f, 1.0f, p.life)));

        if (p.text)
        {
            g.setFont (juce::Font (juce::FontOptions().withName ("Arial").withHeight (12.0f).withStyle ("Bold")));
            g.drawText ("DONK", static_cast<int> (p.x), static_cast<int> (p.y), 45, 16, juce::Justification::centred);
        }
        else
        {
            g.fillEllipse (p.x, p.y, 4.0f + p.life * 5.0f, 4.0f + p.life * 5.0f);
        }
    }

    if (flyingPresetToggle.getToggleState())
    {
        for (const auto& flyer : flyingPresets)
        {
            juce::Graphics::ScopedSaveState save (g);
            g.addTransform (juce::AffineTransform::rotation (flyer.angle, flyer.x + 180.0f, flyer.y + 20.0f));
            const float hue = std::fmod (discoHue + flyer.x * 0.001f + flyer.y * 0.0007f, 1.0f);
            g.setColour (juce::Colour::fromHSV (hue, 0.75f, 1.0f, juce::jlimit (0.0f, 1.0f, flyer.life)));
            g.setFont (juce::Font (juce::FontOptions().withName ("Arial").withHeight (flyer.fontSize).withStyle ("Bold")));
            g.drawText (flyer.text, static_cast<int> (flyer.x), static_cast<int> (flyer.y), 380, 55, juce::Justification::centred);
        }
    }

    g.setColour (
        juce::Colour (0xff61586e));

    g.setFont (juce::Font (juce::FontOptions()
                .withName ("Arial")
                .withHeight (10.0f)));

    g.drawText (
        "100% Free - Made By STARSLAV - SHARE IT - SEED IT - LEECH IT",
        0,
        getHeight() - 16,
        getWidth(),
        15,
        juce::Justification::centred);
}

void NewProjectAudioProcessorEditor::resized()
{
    const int knobWidth = 135;
    const int knobHeight = 100;
    const int spacing = 12;

    const int totalWidth = knobWidth * 4
        +
        spacing * 3;

    const int startX = (getWidth() - totalWidth) / 2;
    const int row1Y = 282;
    const int row2Y = 406;
    const int x1 = startX;

    const int x2 = startX +
        knobWidth +
        spacing;

    const int x3 = startX +
        (knobWidth + spacing) * 2;

    const int x4 = startX +
        (knobWidth + spacing) * 3;

    const bool advancedLayout = audioProcessor.getAdvancedModeEnabled();

    if (advancedLayout)
    {
        const int columns = 5;
        const int startAdvancedX = 26;
        const int topAdvancedY = 278;
        const int cellWidth = (getWidth() - 52) / columns;
        const int cellHeight = 34;

        juce::Slider* mainSliders[] =
        {
            &pitchDropSlider,
            &decaySlider,
            &knockSlider,
            &ratioSlider,
            &shapeSlider,
            &toneSlider,
            &bodySlider,
            &driveSlider
        };

        juce::Label* mainLabels[] =
        {
            &pitchDropLabel,
            &decayLabel,
            &knockLabel,
            &ratioLabel,
            &shapeLabel,
            &toneLabel,
            &bodyLabel,
            &driveLabel
        };

        for (int i = 0; i < 8; ++i)
        {
            const int row = i / columns;
            const int column = i % columns;

            const int x = startAdvancedX
                + column * cellWidth;

            const int y = topAdvancedY
                + row * cellHeight;

            mainLabels[i]->setBounds (
                x,
                y,
                cellWidth - 8,
                12);

            mainLabels[i]->setFont (juce::Font (juce::FontOptions()
                        .withName ("Arial")
                        .withHeight (8.5f)
                        .withStyle ("Bold")));

            mainSliders[i]->setBounds (
                x,
                y + 11,
                cellWidth - 10,
                22);
        }

        const int deepStartY = topAdvancedY
            + 2 * cellHeight
            + 5;

        int visibleAdvancedIndex = 0;

        for (int i = 0;
             i < NewProjectAudioProcessor::AdvancedParameterCount;
             ++i)
        {
            const bool legacyClassic = i >= NewProjectAudioProcessor::ClassicBlend
                && i <= NewProjectAudioProcessor::ClassicELevel;

            if (legacyClassic)
                continue;

            const int row = visibleAdvancedIndex / columns;
            const int column = visibleAdvancedIndex % columns;

            const int x = startAdvancedX
                + column * cellWidth;

            const int y = deepStartY
                + row * cellHeight;

            advancedLabels[static_cast<size_t> (i)].setBounds (
                x,
                y,
                cellWidth - 8,
                12);

            advancedSliders[static_cast<size_t> (i)].setBounds (
                x,
                y + 11,
                cellWidth - 10,
                22);

            ++visibleAdvancedIndex;
        }
    }
    else
    {
    pitchDropLabel.setBounds (
        x1,
        row1Y,
        knobWidth,
        20);

    pitchDropSlider.setBounds (
        x1,
        row1Y + 18,
        knobWidth,
        knobHeight);

    decayLabel.setBounds (
        x2,
        row1Y,
        knobWidth,
        20);

    decaySlider.setBounds (
        x2,
        row1Y + 18,
        knobWidth,
        knobHeight);

    knockLabel.setBounds (
        x3,
        row1Y,
        knobWidth,
        20);

    knockSlider.setBounds (
        x3,
        row1Y + 18,
        knobWidth,
        knobHeight);

    ratioLabel.setBounds (
        x4,
        row1Y,
        knobWidth,
        20);

    ratioSlider.setBounds (
        x4,
        row1Y + 18,
        knobWidth,
        knobHeight);

    shapeLabel.setBounds (
        x1,
        row2Y,
        knobWidth,
        20);

    shapeSlider.setBounds (
        x1,
        row2Y + 18,
        knobWidth,
        knobHeight);

    toneLabel.setBounds (
        x2,
        row2Y,
        knobWidth,
        20);

    toneSlider.setBounds (
        x2,
        row2Y + 18,
        knobWidth,
        knobHeight);

    bodyLabel.setBounds (
        x3,
        row2Y,
        knobWidth,
        20);

    bodySlider.setBounds (
        x3,
        row2Y + 18,
        knobWidth,
        knobHeight);

    driveLabel.setBounds (
        x4,
        row2Y,
        knobWidth,
        20);

    driveSlider.setBounds (
        x4,
        row2Y + 18,
        knobWidth,
        knobHeight);
    }

    declickerToggle.setBounds (
        25,
        66,
        110,
        20);

    declickerStrengthSlider.setBounds (
        136,
        44,
        96,
        22);

    declickerStrengthLabel.setBounds (
        136,
        66,
        96,
        18);

    reverbToggle.setBounds (
        25,
        86,
        110,
        24);

    reverbLabel.setBounds (
        18,
        110,
        145,
        18);

    reverbSlider.setBounds (
        18,
        127,
        145,
        70);

    sidechainToggle.setBounds (
        25,
        199,
        110,
        20);

    sidechainAfterReverbToggle.setBounds (
        136,
        199,
        132,
        20);

    sidechainAmountLabel.setBounds (
        18,
        219,
        52,
        17);

    sidechainAmountSlider.setBounds (
        68,
        216,
        95,
        22);

    sidechainPositionLabel.setBounds (
        18,
        239,
        52,
        17);

    sidechainPositionSlider.setBounds (
        68,
        236,
        95,
        22);

    volumeLabel.setBounds (
        getWidth() - 163,
        108,
        145,
        18);

    volumeSlider.setBounds (
        getWidth() - 163,
        125,
        145,
        70);

    donkLoopSpeedLabel.setBounds (
        getWidth() - 195,
        51,
        92,
        17);

    donkLoopSpeedSlider.setBounds (
        getWidth() - 195,
        62,
        170,
        22);

    donkLoopButton.setBounds (
        getWidth() - 195,
        86,
        170,
        26);

    // experiment kontroller
    experimentalControlToggle.setBounds (
        getWidth() - 195,
        202,
        170,
        20);

    experimentalRandomToggle.setBounds (
        getWidth() - 195,
        225,
        170,
        20);

    animationsOffToggle.setBounds (
        24,
        4,
        175,
        22);

    experimentalFunToggle.setBounds (
        24,
        27,
        175,
        22);

    funPanel.setBounds (18, 50, 265, memePanelOpen ? 382 : 232);
    semechkiRainToggle.setBounds (8, 8, 205, 20);
    babushkaSafeToggle.setBounds (8, 31, 240, 20);
    neighborMeterToggle.setBounds (8, 54, 245, 20);
    redAlertToggle.setBounds (8, 77, 240, 20);
    impactCracksToggle.setBounds (8, 100, 240, 20);
    randomExplosionButton.setBounds (10, 126, 238, 24);
    doNotPressButton.setBounds (10, 155, 238, 26);
    memeButton.setBounds (10, 188, 238, 24);
    closeFunPanelButton.setBounds (229, 5, 26, 22);
    bassEarthquakeToggle.setBounds (8, 220, 240, 20);
    screenShakeToggle.setBounds (8, 243, 240, 20);
    faceSquashToggle.setBounds (8, 266, 240, 20);
    ladaModeToggle.setBounds (8, 289, 240, 20);
    discoModeToggle.setBounds (8, 312, 240, 20);
    flyingPresetToggle.setBounds (8, 335, 245, 20);

    minimalisticToggle.setBounds (
        getWidth() - 173,
        4,
        136,
        22);

    advancedModeToggle.setBounds (
        getWidth() - 173,
        27,
        136,
        22);

    helpButton.setBounds (
        getWidth() - 30,
        4,
        24,
        22);

    const int bottomY = getHeight() - 50;

    giveBackDonkButton.setBounds (
        35,
        bottomY - 28,
        150,
        22);

    randomButton.setBounds (
        35,
        bottomY,
        150,
        25);

    basslineToggle.setBounds (
        195,
        bottomY,
        85,
        25);

    previousPresetButton.setBounds (
        290,
        bottomY,
        35,
        25);

    refreshPresetButton.setBounds (
        330,
        bottomY,
        25,
        25);

    presetBox.setBounds (
        360,
        bottomY,
        audioProcessor.getAdvancedModeEnabled() ? 285 : 175,
        25);

    nextPresetButton.setBounds (
        audioProcessor.getAdvancedModeEnabled() ? 650 : 540,
        bottomY,
        35,
        25);

    savePresetButton.setBounds (
        audioProcessor.getAdvancedModeEnabled() ? 690 : 580,
        bottomY,
        audioProcessor.getAdvancedModeEnabled() ? 195 : 112,
        25);

    // flytta popupen igen om den syns

    if (presetGrid != nullptr
        && presetGrid->isVisible())
    {
        showPresetGrid();
    }

    if (funPanel.isVisible())
        funPanel.toFront (true);

    if (helpPanel != nullptr
        && helpPanel->isVisible())
    {
        helpPanel->setBounds (getLocalBounds());
        helpPanel->toFront (true);
    }
}

juce::Rectangle<int>
NewProjectAudioProcessorEditor::getFaceClickBounds() const
{
    // lite större än ansiktet så de blir enklare o högerklicka
    return juce::Rectangle<int> (
        getWidth() / 2 - 70,
        62,
        140,
        145);
}

juce::Image NewProjectAudioProcessorEditor::loadFaceImageFromFile (const juce::File& file) const
{
    if (! file.existsAsFile())
        return {};

    auto image = juce::ImageFileFormat::loadFrom (
            file);

    if (! image.isValid())
        return {};

    constexpr int maximumStoredDimension = 1024;
    const int width = image.getWidth();
    const int height = image.getHeight();

    const int largestDimension = juce::jmax (
            width,
            height);

    if (largestDimension >
        maximumStoredDimension)
    {
        const double scale = static_cast<double> (
                maximumStoredDimension)
            /
            static_cast<double> (largestDimension);

        const int newWidth = juce::jmax (
                1,
                static_cast<int> (
                    std::round (static_cast<double> (width)
                        *
                        scale)));

        const int newHeight = juce::jmax (
                1,
                static_cast<int> (
                    std::round (static_cast<double> (height)
                        *
                        scale)));

        image = image.rescaled (
                newWidth,
                newHeight,
                juce::Graphics::highResamplingQuality);
    }

    return image;
}

void NewProjectAudioProcessorEditor::loadFaceImages()
{

    starSlavImage = juce::ImageFileFormat::loadFrom (
            BinaryData::STARDONK_png,
            BinaryData::STARDONK_pngSize);

    // förminska inbyggda bilden också om den råkar va skitstor
    if (starSlavImage.isValid())
    {
        constexpr int maximumStoredDimension = 1024;

        const int largestDimension = juce::jmax (
                starSlavImage.getWidth(),
                starSlavImage.getHeight());

        if (largestDimension >
            maximumStoredDimension)
        {
            const double scale = static_cast<double> (
                    maximumStoredDimension)
                /
                static_cast<double> (largestDimension);

            const int newWidth = juce::jmax (
                    1,
                    static_cast<int> (
                        std::round (static_cast<double> (starSlavImage.getWidth())
                            *
                            scale)));

            const int newHeight = juce::jmax (
                    1,
                    static_cast<int> (
                        std::round (static_cast<double> (starSlavImage.getHeight())
                            *
                            scale)));

            starSlavImage = starSlavImage.rescaled (
                    newWidth,
                    newHeight,
                    juce::Graphics::highResamplingQuality);
        }
    }

    const auto savedPath = audioProcessor.getCustomFacePath();

    if (savedPath.isNotEmpty())
    {
        customFaceImage = loadFaceImageFromFile (
                juce::File (savedPath));
    }

    // om sparade custom filen försvunnit så gå tillbaks till starslav bara
    if (audioProcessor.getFaceMode() == NewProjectAudioProcessor::FaceMode::Custom
        &&
        ! customFaceImage.isValid())
    {
        audioProcessor.setFaceMode (NewProjectAudioProcessor::FaceMode::StarSlav);
    }
}

void NewProjectAudioProcessorEditor::selectFaceMode (NewProjectAudioProcessor::FaceMode mode)
{
    if (mode == NewProjectAudioProcessor::FaceMode::StarSlav
        &&
        ! starSlavImage.isValid())
    {
        return;
    }

    if (mode == NewProjectAudioProcessor::FaceMode::Custom
        &&
        ! customFaceImage.isValid())
    {
        chooseCustomFace();
        return;
    }

    audioProcessor.setFaceMode (mode);
    repaint();
}

void NewProjectAudioProcessorEditor::showFaceMenu()
{
    juce::PopupMenu menu;
    const auto currentMode = audioProcessor.getFaceMode();

    menu.addItem (
        1,
        "StarSlav",
        starSlavImage.isValid(),
        currentMode == NewProjectAudioProcessor::FaceMode::StarSlav);

    menu.addItem (
        2,
        "Custom",
        true,
        currentMode == NewProjectAudioProcessor::FaceMode::Custom);

    const auto menuAnchor = localPointToGlobal (
            juce::Point<int> (
                getWidth() / 2 + 58,
                132));

    menu.showMenuAsync (juce::PopupMenu::Options()
            .withTargetScreenArea (juce::Rectangle<int> (menuAnchor.x, menuAnchor.y, 1, 1)), [this] (int result)
        {
            if (result == 1)
            {
                selectFaceMode (NewProjectAudioProcessor::FaceMode::StarSlav);
            }
            else if (result == 2)
            {
                chooseCustomFace();
            }

        });
}

void NewProjectAudioProcessorEditor::chooseCustomFace()
{
    auto picturesDirectory = juce::File::getSpecialLocation (
            juce::File::userDocumentsDirectory)
            .getChildFile ("StarDonk")
            .getChildFile ("Pictures");

    if (! picturesDirectory.exists())
        picturesDirectory.createDirectory();

    faceFileChooser = std::make_unique<juce::FileChooser> (
            "Choose a custom StarDonk face",
            picturesDirectory,
            "*.png");

    const int chooserFlags = juce::FileBrowserComponent::openMode
        |
        juce::FileBrowserComponent::canSelectFiles;

    faceFileChooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& chooser)
        {
            const auto chosenFile = chooser.getResult();

            if (chosenFile.existsAsFile())
            {
                const auto newImage = loadFaceImageFromFile (
                        chosenFile);

                if (newImage.isValid())
                {
                    customFaceImage = newImage;
                    audioProcessor.setCustomFacePath (chosenFile.getFullPathName());
                    audioProcessor.setFaceMode (NewProjectAudioProcessor::FaceMode::Custom);
                    repaint();
                }
            }

            faceFileChooser.reset();
        });
}

void NewProjectAudioProcessorEditor::mouseDown (const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown()
        &&
        getFaceClickBounds().contains (event.getPosition()))
    {
        hidePresetGrid();
        showFaceMenu();
        return;
    }

    if (presetGrid != nullptr
        && presetGrid->isVisible())
    {
        const auto position = event.getPosition();

        const bool clickedGrid = presetGrid->getBounds()
                .contains (position);

        const bool clickedPresetButton = presetBox.getBounds()
                .contains (position);

        if (! clickedGrid
            && ! clickedPresetButton)
        {
            hidePresetGrid();
        }
    }

    grabKeyboardFocus();
    juce::AudioProcessorEditor::mouseDown (event);
}

void NewProjectAudioProcessorEditor::mouseUp (const juce::MouseEvent& event)
{
    juce::AudioProcessorEditor::mouseUp (event);
}


int NewProjectAudioProcessorEditor::getComputerKeyboardNoteForIndex (int index) const
{
    static constexpr int notes[] =
    {
        60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71,
        72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83
    };

    if (index < 0 || index >= static_cast<int> (std::size (notes)))
        return -1;

    return notes[index];
}

void NewProjectAudioProcessorEditor::updateComputerKeyboardState()
{
    if (! hasKeyboardFocus (true))
    {
        computerKeyboardHeld.fill (false);
        computerKeyboardPressOrder.fill (0);
        audioProcessor.setComputerKeyboardNote (-1);
        return;
    }

    static constexpr int keyCodes[] =
    {
        'z', 's', 'x', 'd', 'c', 'v', 'g', 'b', 'h', 'n', 'j', 'm',
        'q', '2', 'w', '3', 'e', 'r', '5', 't', '6', 'y', '7', 'u'
    };

    bool changed = false;

    for (int i = 0; i < static_cast<int> (std::size (keyCodes)); ++i)
    {
        const bool isDown = juce::KeyPress::isKeyCurrentlyDown (keyCodes[i]);

        if (computerKeyboardHeld[static_cast<size_t> (i)] != isDown)
        {
            computerKeyboardHeld[static_cast<size_t> (i)] = isDown;
            changed = true;

            if (isDown)
                computerKeyboardPressOrder[static_cast<size_t> (i)] = ++computerKeyboardOrderCounter;
            else
                computerKeyboardPressOrder[static_cast<size_t> (i)] = 0;
        }
    }

    if (! changed)
        return;

    int newestIndex = -1;
    uint64_t newestOrder = 0;

    for (int i = 0; i < static_cast<int> (computerKeyboardHeld.size()); ++i)
    {
        if (computerKeyboardHeld[static_cast<size_t> (i)]
            && computerKeyboardPressOrder[static_cast<size_t> (i)] >= newestOrder)
        {
            newestOrder = computerKeyboardPressOrder[static_cast<size_t> (i)];
            newestIndex = i;
        }
    }

    audioProcessor.setComputerKeyboardNote (
        newestIndex >= 0 ? getComputerKeyboardNoteForIndex (newestIndex) : -1);
}

bool NewProjectAudioProcessorEditor::keyPressed (const juce::KeyPress& key)
{
    updateComputerKeyboardState();

    const int keyCode = juce::CharacterFunctions::toLowerCase (
        static_cast<juce::juce_wchar> (key.getKeyCode()));

    static constexpr int mappedKeys[] =
    {
        'z', 's', 'x', 'd', 'c', 'v', 'g', 'b', 'h', 'n', 'j', 'm',
        'q', '2', 'w', '3', 'e', 'r', '5', 't', '6', 'y', '7', 'u'
    };

    for (const auto mappedKey : mappedKeys)
        if (keyCode == mappedKey)
            return true;

    return juce::AudioProcessorEditor::keyPressed (key);
}

bool NewProjectAudioProcessorEditor::keyStateChanged (bool isKeyDown)
{
    juce::ignoreUnused (isKeyDown);
    updateComputerKeyboardState();
    return true;
}
