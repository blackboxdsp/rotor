/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   louisgeorgecafe_ttf;
    const int            louisgeorgecafe_ttfSize = 32020;

    extern const char*   louisgeorgecafelight_ttf;
    const int            louisgeorgecafelight_ttfSize = 32552;

    extern const char*   montserrat_ttf;
    const int            montserrat_ttfSize = 245708;

    extern const char*   montserratlight_ttf;
    const int            montserratlight_ttfSize = 242068;

    extern const char*   backgroundlight_png;
    const int            backgroundlight_pngSize = 98044;

    extern const char*   background_png;
    const int            background_pngSize = 106829;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
