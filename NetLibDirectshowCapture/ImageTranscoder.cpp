#include "pch.h"
#include "NetLibDirectshowCapture.h"

int NetLibDirectshowCapture::ImageTranscoder::XrgbToBgr24(void* nativeSrc, int nativeLength, array<Byte>^ managedDst, int width, int height)
{
    int stride = 3 * width; // BGR24 is 3-byte per pixel
    int baseIndex = 0;
    int copyIndex = stride - 1;
    unsigned char* src = static_cast<unsigned char*>(nativeSrc);
    // Flip y, Flip x
    for (int i = nativeLength - 1 - 4; i >= 0; i -= 4)
    {
        // R
        managedDst[baseIndex + copyIndex--] = src[i + 3];
        // G
        managedDst[baseIndex + copyIndex--] = src[i + 2];
        // B
        managedDst[baseIndex + copyIndex--] = src[i + 1];
        if (copyIndex < 0)
        {
            baseIndex += stride;
            copyIndex = stride - 1;
        }
    }
    return stride * height;
}