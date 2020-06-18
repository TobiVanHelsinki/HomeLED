#pragma once

#if _DEBUG
#define SERIALWRITELINE(x) SERIALWRITELINE(x);
#define SERIALWRITE(x) SERIALWRITE(x);
#else
#define SERIALWRITELINE(x)
#define SERIALWRITE(x)
#endif // _DEBUG

/*
For OTA: set FS to none
*/