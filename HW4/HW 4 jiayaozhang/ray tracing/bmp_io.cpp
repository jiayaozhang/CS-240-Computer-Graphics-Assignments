# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <fstream>

using namespace std;

# include "bmp_io.h"

static bool bmp_byte_swap = true;



bool bmp_byte_swap_get ( void )


//
{
  return bmp_byte_swap;
}


void bmp_byte_swap_set ( bool value )

//
{
  bmp_byte_swap = value;

  return;
}

bool bmp_08_data_read ( ifstream &file_in, unsigned long int width, long int height, 
  unsigned char *rarray )

//
{
  char c;
  bool error;
  int i;
  unsigned char *indexr;
  int j;
  int numbyte;
  int padding;
//
//  Set the padding.
//
  padding = ( 4 - ( ( 1 * width ) % 4 ) ) % 4;

  indexr = rarray;
  numbyte = 0;

  for ( j = 0; j < abs ( height ); j++ ) 
  {
    for ( i = 0; i < width; i++ )
    {
      file_in.read ( &c, 1 );

      error = file_in.eof();

      if ( error ) 
      {
        cout << "\n";
        cout << "BMP_08_DATA_READ: Fatal error!\n";
        cout << "  Failed reading R for pixel (" << i << "," << j << ").\n";
        return error;
      }

      *indexr = ( unsigned char ) c;
      numbyte = numbyte + 1;
      indexr = indexr + 1;
    }
//
//  If necessary, read a few padding characters.
//
    for ( i = 0; i < padding; i++ )
    {

      file_in.read ( &c, 1 );

      error = file_in.eof();

      if ( error )
      {
        cout << "\n";
        cout << "BMP_08_DATA_READ - Warning!\n";
        cout << "  Failed while reading padding character " << i << "\n";
        cout << "  of total " << padding << " characters\n";
        cout << "  at the end of line " << j << "\n";
        cout << "\n";
        cout << "  This is a minor error.\n";
        return false;
      }
    }
  }

  return false;
}
//****************************************************************************

void bmp_08_data_write ( ofstream &file_out, unsigned long int width, 
  long int height, unsigned char *rarray )

//
{
  int i;
  unsigned char *indexr;
  int j;
  int padding;
//
//  Set the padding.
//
  padding = ( 4 - ( ( 1 * width ) % 4 ) ) % 4;

  indexr = rarray;

  for ( j = 0; j < abs ( height ); j++ )
  {
    for ( i = 0; i < width; i++ )
    {
      file_out << *indexr;
      indexr = indexr + 1;
    }

    for ( i = 0; i < padding; i++ )
    {
      file_out << 0;
    }
  }

  return;
}

bool bmp_24_data_read ( ifstream &file_in, unsigned long int width, long int height, 
  unsigned char *rarray, unsigned char *garray, unsigned char *barray )

//
{
  char c;
  bool error;
  int i;
  unsigned char *indexb;
  unsigned char *indexg;
  unsigned char *indexr;
  int j;
  int numbyte;
  int padding;
//
//  Set the padding.
//
  padding = ( 4 - ( ( 3 * width ) % 4 ) ) % 4;

  indexr = rarray;
  indexg = garray;
  indexb = barray;
  numbyte = 0;

  for ( j = 0; j < abs ( height ); j++ ) 
  {
    for ( i = 0; i < width; i++ )
    {

      file_in.read ( &c, 1 );

      error = file_in.eof();

      if ( error )
      {
        cout << "\n";
        cout << "BMP_24_DATA_READ: Fatal error!\n";
        cout << "  Failed reading B for pixel (" << i << "," << j << ").\n";
        return error;
      }

      *indexb = ( unsigned char ) c;
      numbyte = numbyte + 1;
      indexb = indexb + 1;

      file_in.read ( &c, 1 );

      error = file_in.eof();

      if ( error ) 
      {
        cout << "\n";
        cout << "BMP_24_DATA_READ: Fatal error!\n";
        cout << "  Failed reading G for pixel (" << i << "," << j << ").\n";
        return error;
      }

      *indexg = ( unsigned char ) c;
      numbyte = numbyte + 1;
      indexg = indexg + 1;

      file_in.read ( &c, 1 );

      error = file_in.eof();

      if ( error ) 
      {
        cout << "\n";
        cout << "BMP_24_DATA_READ: Fatal error!\n";
        cout << "  Failed reading R for pixel (" << i << "," << j << ").\n";
        return error;
      }

      *indexr = ( unsigned char ) c;
      numbyte = numbyte + 1;
      indexr = indexr + 1;
    }
//
//
    for ( i = 0; i < padding; i++ )
    {

      file_in.read ( &c, 1 );

      error = file_in.eof();

      if ( error )
      {
        cout << "\n";
        cout << "BMP_24_DATA_READ - Warning!\n";
        cout << "  Failed while reading padding character " << i << "\n";
        cout << "  of total " << padding << " characters\n";
        cout << "  at the end of line " << j << "\n";
        cout << "\n";
        cout << "  This is a minor error.\n";
        return false;
      }
    }
  }

  return false;
}

void bmp_24_data_write ( ofstream &file_out, unsigned long int width, 
  long int height, unsigned char *rarray, unsigned char *garray, 
  unsigned char *barray )

//****************************************************************************
{
  int i;
  unsigned char *indexb;
  unsigned char *indexg;
  unsigned char *indexr;
  int j;
  int padding;
//
//  Set the padding.
//
  padding = ( 4 - ( ( 3 * width ) % 4 ) ) % 4;

  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for ( j = 0; j < abs ( height ); j++ )
  {
    for ( i = 0; i < width; i++ )
    {
      file_out << *indexb;
      file_out << *indexg;
      file_out << *indexr;

      indexb = indexb + 1;
      indexg = indexg + 1;
      indexr = indexr + 1;
    }

    for ( i = 0; i < padding; i++ )
    {
      file_out << 0;
    }
  }

  return;
}
//****************************************************************************

void bmp_header1_print ( unsigned short int filetype, 
  unsigned long int filesize, unsigned short int reserved1, 
  unsigned short int reserved2, unsigned long int bitmapoffset )

//
{
  cout << "\n";
  cout << "  Contents of BMP file header:\n";
  cout << "\n";
  cout << "    FILETYPE =     " << filetype     << "\n";
  cout << "    FILESIZE =     " << filesize     << "\n";
  cout << "    RESERVED1 =    " << reserved1    << "\n";
  cout << "    RESERVED2 =    " << reserved2    << "\n";
  cout << "    BITMAPOFFSET = " << bitmapoffset << "\n";

  return;
}
//****************************************************************************

bool bmp_header1_read ( ifstream &file_in, unsigned short int *filetype, 
  unsigned long int *filesize, unsigned short int *reserved1, 
  unsigned short int *reserved2, unsigned long int *bitmapoffset )

//****************************************************************************
//
{
  bool error;
  char i1;
  char i2;  
//
//
  error = u_short_int_read ( filetype, file_in );

  if ( error )
  {
    return error;
  }
//
//
  if ( bmp_byte_swap )
  {
    i1 = ( char ) ( *filetype / 256 );
    i2 = ( char ) ( *filetype % 256 );
    *filetype = i2 * 256 + i1;
  }
//
//
  error = u_long_int_read ( filesize, file_in );
  if ( error ) 
  {
    return error;
  }
//
//
  error = u_short_int_read ( reserved1, file_in );
  if ( error )
  {
    return error;
  }
//
//
  error = u_short_int_read ( reserved2, file_in );
  if ( error )
  {
    return error;
  }
//
//
  error = u_long_int_read ( bitmapoffset, file_in );
  if ( error )
  {
    return error;
  }

  error = false;
  return error;
}

void bmp_header1_write ( ofstream &file_out, unsigned short int filetype,
  unsigned long int filesize, unsigned short int reserved1, 
  unsigned short int reserved2, unsigned long int bitmapoffset )

//
{
  u_short_int_write ( filetype, file_out );
  u_long_int_write ( filesize, file_out );
  u_short_int_write ( reserved1, file_out );
  u_short_int_write ( reserved2, file_out );
  u_long_int_write ( bitmapoffset, file_out );

  return;
}

void bmp_header2_print ( unsigned long int size, unsigned long int width, 
  long int height, 
  unsigned short int planes, unsigned short int bitsperpixel, 
  unsigned long int compression, unsigned long int sizeofbitmap,
  unsigned long int horzresolution, unsigned long int vertresolution,
  unsigned long int colorsused,  unsigned long int colorsimportant )

{
  cout << "\n";
  cout << "  Contents of BMP file bitmap header:\n";
  cout << "\n";
  cout << "    SIZE =            " << size            << "\n";
  cout << "    WIDTH =           " << width           << "\n";
  cout << "    HEIGHT =          " << height          << "\n";
  cout << "    PLANES =          " << planes          << "\n";
  cout << "    BITSPERPIXEL =    " << bitsperpixel    << "\n";
  cout << "    COMPRESSION =     " << compression     << "\n";
  cout << "    SIZEOFBITMAP =    " << sizeofbitmap    << "\n";
  cout << "    HORZRESOLUTION =  " << horzresolution  << "\n";
  cout << "    VERTRESOLUTION =  " << vertresolution  << "\n";
  cout << "    COLORSUSED =      " << colorsused      << "\n";
  cout << "    COLORSIMPORTANT = " << colorsimportant << "\n";

  return;
} 


bool bmp_header2_read ( ifstream &file_in, unsigned long int *size,
  unsigned long int *width, long int *height, 
  unsigned short int *planes, unsigned short int *bitsperpixel,
  unsigned long int *compression, unsigned long int *sizeofbitmap,
  unsigned long int *horzresolution, unsigned long int *vertresolution,
  unsigned long int *colorsused, unsigned long int *colorsimportant )

//
{
  bool error;
//
  error = u_long_int_read ( size, file_in );
  if ( error )
  {
    return error;
  }
//
  error = u_long_int_read ( width, file_in );
  if ( error )
  {
    return error;
  }
//
//
  error = long_int_read ( height, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read PLANES, the number of color planes.
//
  error = u_short_int_read ( planes, file_in ); 
  if ( error )
  {
    return error;
  }
//
//  Read BITSPERPIXEL.
//
  error = u_short_int_read ( bitsperpixel, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read COMPRESSION.
//
  error = u_long_int_read ( compression, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read SIZEOFBITMAP.
//
  error = u_long_int_read ( sizeofbitmap, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read HORZRESOLUTION.
//
  error = u_long_int_read ( horzresolution, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read VERTRESOLUTION.
//
  error = u_long_int_read ( vertresolution, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read COLORSUSED.
//
  error = u_long_int_read ( colorsused, file_in );
  if ( error )
  {
    return error;
  }
//
//  Read COLORSIMPORTANT.
//
  error = u_long_int_read ( colorsimportant, file_in );
  if ( error )
  {
    return error;
  }

  error = false;
  return error;
}
//****************************************************************************

void bmp_header2_write ( ofstream &file_out, unsigned long int size,
  unsigned long int width, long int height, 
  unsigned short int planes, unsigned short int bitsperpixel,
  unsigned long int compression, unsigned long int sizeofbitmap,
  unsigned long int horzresolution, unsigned long int vertresolution,
  unsigned long int colorsused, unsigned long int colorsimportant )

//****************************************************************************
//
{
  u_long_int_write ( size, file_out );
  u_long_int_write ( width, file_out );
  long_int_write ( height, file_out );
  u_short_int_write ( planes, file_out ); 
  u_short_int_write ( bitsperpixel, file_out );
  u_long_int_write ( compression, file_out );
  u_long_int_write ( sizeofbitmap, file_out );
  u_long_int_write ( horzresolution, file_out );
  u_long_int_write ( vertresolution, file_out );
  u_long_int_write ( colorsused, file_out );
  u_long_int_write ( colorsimportant, file_out );

  return;
}
//****************************************************************************

void bmp_palette_print ( unsigned long int colorsused, 
  unsigned char *rparray, unsigned char *gparray, unsigned char *bparray,
  unsigned char *aparray )

//****************************************************************************
{
  int i;
  unsigned char *indexa;
  unsigned char *indexb;
  unsigned char *indexg;
  unsigned char *indexr;

  cout << "\n";
  cout << "  Palette information from BMP file:\n";
  cout << "\n";

  if ( colorsused < 1 )
  {
    cout << "    There are NO colors defined for the palette.\n";
    return;
  }

  indexr = rparray;
  indexg = gparray;
  indexb = bparray;
  indexa = aparray;

  cout << "\n";
  cout << " Color    Blue   Green     Red   Trans\n";
  cout << "\n";

  for ( i = 0; i < colorsused; i++ )
  {
    cout << setw(6) << i       << "  "
         << setw(6) << *indexb << "  "
         << setw(6) << *indexg << "  "
         << setw(6) << *indexr << "  "
         << setw(6) << *indexa << "\n";

    indexb = indexb + 1;
    indexg = indexg + 1;
    indexr = indexr + 1;
    indexa = indexa + 1;

  }

  return;
}
//****************************************************************************

bool bmp_palette_read ( ifstream &file_in, unsigned long int colorsused,
  unsigned char *rparray, unsigned char *gparray, unsigned char *bparray, 
  unsigned char *aparray )

//****************************************************************************
//
{
  char c;
  bool error;
  int i;
  unsigned char *indexa;
  unsigned char *indexb;
  unsigned char *indexg;
  unsigned char *indexr;

  indexr = rparray;
  indexg = gparray;
  indexb = bparray;
  indexa = aparray;

  for ( i = 0; i < colorsused; i++ )
  {

    file_in.read ( &c, 1 );

    error = file_in.eof();

    if ( error )
    {
      cout << "\n";
      cout << "BMP_PALETTE_READ: Fatal error!\n";
      cout << "  Failed reading B for palette color " << i << ".\n";
      return error;
    }

    *indexb = ( unsigned char ) c;
    indexb = indexb + 1;

    file_in.read ( &c, 1 );

    error = file_in.eof();

    if ( error )
    {
      cout << "\n";
      cout << "BMP_PALETTE_READ: Fatal error!\n";
      cout << "  Failed reading G for palette color " << i << ".\n";
      return error;
    }

    *indexg = ( unsigned char ) c;
    indexg = indexg + 1;

    file_in.read ( &c, 1 );

    error = file_in.eof();

    if ( error )
    {
      cout << "\n";
      cout << "BMP_PALETTE_READ: Fatal error!\n";
      cout << "  Failed reading R for palette color " << i << ".\n";
      return error;
    }

    *indexr = ( unsigned char ) c;
    indexr = indexr + 1;

    file_in.read ( &c, 1 );

    error = file_in.eof();

    if ( error )
    {
      cout << "\n";
      cout << "BMP_PALETTE_READ: Fatal error!\n";
      cout << "  Failed reading A for palette color " << i << ".\n";
      return error;
    }

    *indexa = ( unsigned char ) c;
    indexa = indexa + 1;
  }

  error = false;
  return error;
}
//****************************************************************************

void bmp_palette_write ( ofstream &file_out, unsigned long int colorsused, 
  unsigned char *rparray, unsigned char *gparray, unsigned char *bparray,
  unsigned char *aparray )

//****************************************************************************
//
{
  int i;
  unsigned char *indexa;
  unsigned char *indexb;
  unsigned char *indexg;
  unsigned char *indexr;

  indexr = rparray;
  indexg = gparray;
  indexb = bparray;
  indexa = aparray;

  for ( i = 0; i < colorsused; i++ )
  {
    file_out << *indexb;
    file_out << *indexg;
    file_out << *indexr;
    file_out << *indexa;

    indexb = indexb + 1;
    indexg = indexg + 1;
    indexr = indexr + 1;
    indexa = indexa + 1;
  }

  return;
}
//****************************************************************************

bool bmp_print_test ( char *file_in_name )

//****************************************************************************
//
{
# define VERBOSE false

  unsigned char *aparray;
  unsigned char *barray;
  unsigned char *bparray;
  unsigned long int bitmapoffset;
  unsigned short int bitsperpixel;
  unsigned long int colorsimportant;
  unsigned long int colorsused;
  unsigned long int compression;
  bool error;
  ifstream file_in;
  unsigned long int filesize;
  unsigned short int filetype;
  unsigned char *garray;
  unsigned char *gparray;
  long int height;
  unsigned long int horzresolution;
  int numbytes;
  unsigned short int planes;
  unsigned char *rarray;
  unsigned char *rparray;
  unsigned short int reserved1;
  unsigned short int reserved2;
  unsigned long int size;
  unsigned long int sizeofbitmap;
  unsigned long int vertresolution;
  unsigned long int width;
//
//  Open the input file.
//
  file_in.open ( file_in_name, ios::in | ios::binary );

  if ( !file_in ) 
  {
    error = true;
    cout << "\n";
    cout << "BMP_PRINT_TEST - Fatal error!\n";
    cout << "  Could not open the input file.\n";
    return error;
  }
  cout << "\n";
  cout << "BMP_PRINT_TEST:\n";
  cout << "  Contents of BMP file \"" << file_in_name << "\"\n";
//
//  Read header 1.
//
  error = bmp_header1_read ( file_in, &filetype, &filesize, &reserved1, 
    &reserved2, &bitmapoffset );

  if ( error ) 
  {
    cout << "\n";
    cout << "BMP_PRINT_TEST: Fatal error!\n";
    cout << "  BMP_HEADER1_READ failed.\n";
    return error;
  }

  bmp_header1_print ( filetype, filesize, reserved1, reserved2, bitmapoffset );
//
//  Read header 2.
//
  error = bmp_header2_read ( file_in, &size, &width, &height, &planes,
    &bitsperpixel, &compression, &sizeofbitmap, &horzresolution,
    &vertresolution, &colorsused, &colorsimportant );

  if ( error ) 
  {
    cout << "\n";
    cout << "BMP_PRINT_TEST: Fatal error!\n";
    cout << "  BMP_HEADER2_READ failed.\n";
    return error;
  }

  bmp_header2_print ( size, width, height, planes, bitsperpixel,
    compression, sizeofbitmap, horzresolution, vertresolution, 
    colorsused, colorsimportant );
//
//  Read the palette.
//
//if ( 0 < colorsused )
//{
    rparray = new unsigned char[colorsused];
    gparray = new unsigned char[colorsused];
    bparray = new unsigned char[colorsused];
    aparray = new unsigned char[colorsused];

    error = bmp_palette_read ( file_in, colorsused, rparray, gparray,
      bparray, aparray );

    if ( error ) 
    {
      cout << "\n";
      cout << "BMP_PRINT_TEST: Fatal error!\n";
      cout << "  BMP_PALETTE_READ failed.\n";
      return error;
    }

    bmp_palette_print ( colorsused, rparray, gparray, bparray, aparray );

    delete [] rparray;
    delete [] gparray;
    delete [] bparray;
    delete [] aparray;
//}
//
//  Allocate storage.
//
  numbytes = width * abs ( height ) * sizeof ( unsigned char );

  rarray = new unsigned char[numbytes];
  garray = new unsigned char[numbytes];
  barray = new unsigned char[numbytes];
//
//  Read the data.
//
  if ( bitsperpixel == 8 )
  {
    error = bmp_08_data_read ( file_in, width, height, rarray );

    if ( error ) 
    {
      cout << "\n";
      cout << "BMP_PRINT_TEST: Fatal error!\n";
      cout << "  BMP_08_DATA_READ failed.\n";
      return error;
    }

    *garray = *rarray;
    *barray = *rarray;
  }
  else if ( bitsperpixel == 24 )
  {
    error = bmp_24_data_read ( file_in, width, height, rarray, garray, 
      barray );

    if ( error ) 
    {
      cout << "\n";
      cout << "BMP_PRINT_TEST: Fatal error!\n";
      cout << "  BMP_24_DATA_READ failed.\n";
      return error;
    }
  }
  else
  {
    cout << "\n";
    cout << "BMP_PRINT_TEST: Fatal error!\n";
    cout << "  Unrecognized value of BITSPERPIXEL = " << bitsperpixel << "\n";
    return 1;
  }

  delete [] rarray;
  delete [] garray;
  delete [] barray;
//
//  Close the file.
//
  file_in.close ( );

  return error;
# undef VERBOSE
}
//****************************************************************************

bool bmp_read ( char *file_in_name, unsigned long int *width, long int *height, 
  unsigned char **rarray, unsigned char **garray, unsigned char **barray )

//****************************************************************************
//
{
  unsigned char *aparray;
  unsigned long int bitmapoffset;
  unsigned short int bitsperpixel;
  unsigned char *bparray;
  unsigned long int colorsimportant;
  unsigned long int colorsused;
  unsigned long int compression;
  bool error;
  ifstream file_in;
  unsigned long int filesize;
  unsigned short int filetype;
  unsigned char *gparray;
  unsigned long int horzresolution;
  unsigned short int magic;
  int numbytes;
  unsigned short int planes;
  unsigned short int reserved1;
  unsigned short int reserved2;
  unsigned char *rparray;
  unsigned long int size;
  unsigned long int sizeofbitmap;
  unsigned long int vertresolution;
//
//  Open the input file.
//
  file_in.open ( file_in_name, ios::in | ios::binary );

  if ( !file_in ) 
  {
    error = true;
    cout << "\n";
    cout << "BMP_READ - Fatal error!\n";
    cout << "  Could not open the input file.\n";
    return error;
  }
//
//  Read header 1.
//
  error = bmp_header1_read ( file_in, &filetype, &filesize, &reserved1, 
    &reserved2, &bitmapoffset );

  if ( error ) 
  {
    cout << "\n";
    cout << "BMP_READ: Fatal error!\n";
    cout << "  BMP_HEADER1_READ failed.\n";
    return error;
  }
//
//  Make sure the filetype is 'BM'.
//
  magic = 'B' * 256 + 'M';

  if ( filetype != magic )
  {
    cout << "\n";
    cout << "BMP_READ: Fatal error!\n";
    cout << "  The file's internal magic number is not \"BM\".\n";
    cout << "  with the numeric value " << magic << "\n";
    cout << "\n";
    cout << "  Instead, it is \"" 
         << ( char ) ( filetype / 256 ) 
         << ( char ) ( filetype % 256 )
         << "\".\n";
    cout << "  with the numeric value " << filetype << "\n";
    cout << "\n";
    cout << "  (Perhaps you need to reverse the byte swapping option!)\n";
    return 1;
  }
//
//  Read header 2.
//
  error = bmp_header2_read ( file_in, &size, width, height, &planes,
    &bitsperpixel, &compression, &sizeofbitmap, &horzresolution,
    &vertresolution, &colorsused, &colorsimportant );

  if ( error ) 
  {
    cout << "\n";
    cout << "BMP_READ: Fatal error!\n";
    cout << "  BMP_HEADER2_READ failed.\n";
    return error;
  }
//
//  Read the palette.
//
  if ( 0 < colorsused )
  {
    rparray = new unsigned char[colorsused];
    gparray = new unsigned char[colorsused];
    bparray = new unsigned char[colorsused];
    aparray = new unsigned char[colorsused];

    error = bmp_palette_read ( file_in, colorsused, rparray, gparray,
      bparray, aparray );

    if ( error ) 
    {
      cout << "\n";
      cout << "BMP_READ: Fatal error!\n";
      cout << "  BMP_PALETTE_READ failed.\n";
      return error;
    }
    delete [] rparray;
    delete [] gparray;
    delete [] bparray;
    delete [] aparray;
  }
//
//  Allocate storage.
//
  numbytes = ( *width ) * ( abs ( *height ) ) * sizeof ( unsigned char );

  *rarray = new unsigned char[numbytes];
  *garray = new unsigned char[numbytes];
  *barray = new unsigned char[numbytes];
//
//  Read the data.
//
  if ( bitsperpixel == 8 )
  {
    error = bmp_08_data_read ( file_in, *width, *height, *rarray );

    if ( error ) 
    {
      cout << "\n";
      cout << "BMP_READ: Fatal error!\n";
      cout << "  BMP_08_DATA_READ failed.\n";
      return error;
    }

    *garray = *rarray;
    *barray = *rarray;
  }
  else if ( bitsperpixel == 24 )
  {
    error = bmp_24_data_read ( file_in, *width, *height, *rarray, *garray, 
      *barray );

    if ( error ) 
    {
      cout << "\n";
      cout << "BMP_READ: Fatal error!\n";
      cout << "  BMP_24_DATA_READ failed.\n";
      return error;
    }
  }
  else
  {
    cout << "\n";
    cout << "BMP_READ: Fatal error!\n";
    cout << "  Unrecognized value of BITSPERPIXEL = " << bitsperpixel << "\n";
    return 1;
  }
//
//  Close the file.
//
  file_in.close ( );

  error = false;
  return error;
}
//****************************************************************************

bool bmp_read_test ( char *file_in_name )

//****************************************************************************
//
{
# define VERBOSE false

  unsigned char *barray;
  bool error;
  unsigned char *garray;
  long int height;
  unsigned char *rarray;
  unsigned long int width;

  rarray = NULL;
  garray = NULL;
  barray = NULL;
//
//  Read the data from file.
//
  error = bmp_read ( file_in_name, &width, &height, &rarray, &garray, 
    &barray );
//
//  Free the memory.
//
  delete [] rarray;
  delete [] garray;
  delete [] barray;

  if ( VERBOSE )
  {
    if ( error )
    {
      cout << "\n";
      cout << "BMP_READ_TEST - Fatal error!\n";
      cout << "  The test failed.\n";
    }
    else
    {
      cout << "\n";
      cout << "BMP_READ_TEST:\n";
      cout << "  WIDTH =  " << width  << ".\n";
      cout << "  HEIGHT = " << height << ".\n";
      cout << "\n";
      cout << "BMP_READ_TEST:\n";
      cout << "  The test was successful.\n";
    }
  }

  return error;
# undef VERBOSE
}
bool bmp_write ( char *file_out_name, unsigned long int width, long int height, 
  unsigned char *rarray, unsigned char *garray, unsigned char *barray ) {
	return bmp_24_write( file_out_name, width, height, rarray, garray, barray);
}
//****************************************************************************

bool bmp_08_write ( char *file_out_name, unsigned long int width, long int height, 
  unsigned char *rarray )

//****************************************************************************
//
{
  unsigned char *aparray = NULL;
  unsigned long int bitmapoffset;
  unsigned short int bitsperpixel;
  unsigned char *bparray = NULL;
  unsigned long int colorsimportant;
  unsigned long int colorsused;
  unsigned long int compression;
  bool error;
  ofstream file_out;
  unsigned long int filesize;
  unsigned short int filetype;
  unsigned char *gparray = NULL;
  unsigned long int horzresolution;
  int padding;
  unsigned short int planes;
  unsigned short int reserved1 = 0;
  unsigned short int reserved2 = 0;
  unsigned char *rparray = NULL;
  unsigned long int size = 40;
  unsigned long int sizeofbitmap;
  unsigned long int vertresolution;
//
//  Open the output file.
//
  file_out.open ( file_out_name, ios::out | ios::binary );

  error = !file_out;

  if ( error )
  {
    cout << "\n";
    cout << "BMP_08_WRITE - Fatal error!\n";
    cout << "  Could not open the output file.\n";
    return error;
  }
//
//  Write header 1.
//
  if ( bmp_byte_swap )
  {
    filetype = 'M' * 256 + 'B';
  }
  else
  {
    filetype = 'B' * 256 + 'M';
  }
//
//  Determine the padding needed when WIDTH is not a multiple of 4.
//
  padding = ( 4 - ( ( 1 * width ) % 4 ) ) % 4;

  filesize = 54 + ( width + padding ) * abs ( height );
  bitmapoffset = 54;

  bmp_header1_write ( file_out, filetype, filesize, reserved1, 
    reserved2, bitmapoffset );
//
//  Write header 2.
//
  planes = 1;
  bitsperpixel = 8;
  compression = 0;
  sizeofbitmap = 0;
  horzresolution = 0;
  vertresolution = 0;
  colorsused = 0;
  colorsimportant = 0;

  bmp_header2_write ( file_out, size, width, height, planes, bitsperpixel, 
    compression, sizeofbitmap, horzresolution, vertresolution,
    colorsused, colorsimportant );
//
//  Write the palette.
//
  bmp_palette_write ( file_out, colorsused, rparray, gparray, bparray, 
    aparray );
//
//  Write the data.
//
  bmp_08_data_write ( file_out, width, height, rarray );
//
//  Close the file.
//
  file_out.close ( );

  error = false;
  return error;
}
//****************************************************************************

bool bmp_08_write_test ( char *file_out_name )

//****************************************************************************
//
//
{
# define VERBOSE false

  bool error;
  long int height;
  int i;
  unsigned char *indexr;
  int j;
  int numbytes;
  unsigned char *rarray;
  unsigned long int width;

  width = 255;
  height = 255;
//
//  Allocate the memory.
//
  numbytes = width * abs ( height ) * sizeof ( unsigned char );  

  rarray = new unsigned char[numbytes];
//
//  Set the data.
//
  indexr = rarray;

  for ( j = 0; j < height; j++ )
  {
    for ( i = 0; i < width; i++ )
    {
      *indexr = i % ( j + 1 );
      indexr = indexr + 1;
    }
  }
//
//  Write the data to a file.
//
  error = bmp_08_write ( file_out_name, width, height, rarray );

  if ( error )
  {
    cout << "\n";
    cout << "BMP_08_WRITE_TEST - Fatal error!\n";
    cout << "  The test failed.\n";
    return error;
  }
//
//  Free the memory.
//
  delete [] rarray;

  if ( VERBOSE )
  {
    cout << "\n";
    cout << "BMP_08_WRITE_TEST:\n";
    cout << "  The test was successful.\n";
  }

  error = false;
  return error;
# undef VERBOSE
}
//****************************************************************************

bool bmp_24_write ( char *file_out_name, unsigned long int width, long int height, 
  unsigned char *rarray, unsigned char *garray, unsigned char *barray )

//
{
  unsigned char *aparray = NULL;
  unsigned long int bitmapoffset;
  unsigned short int bitsperpixel;
  unsigned char *bparray = NULL;
  unsigned long int colorsimportant;
  unsigned long int colorsused;
  unsigned long int compression;
  bool error;
  ofstream file_out;
  unsigned long int filesize;
  unsigned short int filetype;
  unsigned char *gparray = NULL;
  unsigned long int horzresolution;
  int padding;
  unsigned short int planes;
  unsigned short int reserved1 = 0;
  unsigned short int reserved2 = 0;
  unsigned char *rparray = NULL;
  unsigned long int size = 40;
  unsigned long int sizeofbitmap;
  unsigned long int vertresolution;
//
//  Open the output file.
//
  file_out.open ( file_out_name, ios::out | ios::binary );

  error = !file_out;

  if ( error )
  {
    cout << "\n";
    cout << "BMP_24_WRITE - Fatal error!\n";
    cout << "  Could not open the output file.\n";
    return error;
  }
//
//  Write header 1.
//
  if ( bmp_byte_swap )
  {
    filetype = 'M' * 256 + 'B';
  }
  else
  {
    filetype = 'B' * 256 + 'M';
  }
//
//  Determine the padding needed when WIDTH is not a multiple of 4.
//
  padding = ( 4 - ( ( 3 * width ) % 4 ) ) % 4;

  filesize = 54 + ( ( 3 * width ) + padding ) * abs ( height );
  bitmapoffset = 54;

  bmp_header1_write ( file_out, filetype, filesize, reserved1, 
    reserved2, bitmapoffset );
//
//  Write header 2.
//
  planes = 1;
  bitsperpixel = 24;
  compression = 0;
  sizeofbitmap = 0;
  horzresolution = 0;
  vertresolution = 0;
  colorsused = 0;
  colorsimportant = 0;

  bmp_header2_write ( file_out, size, width, height, planes, bitsperpixel, 
    compression, sizeofbitmap, horzresolution, vertresolution,
    colorsused, colorsimportant );
//
//  Write the palette.
//
  bmp_palette_write ( file_out, colorsused, rparray, gparray, bparray, 
    aparray );
//
//  Write the data.
//
  bmp_24_data_write ( file_out, width, height, rarray, garray, barray );
//
//  Close the file.
//
  file_out.close ( );

  error = false;
  return error;
}
//****************************************************************************

bool bmp_24_write_test ( char *file_out_name )

//****************************************************************************
{
# define VERBOSE false

  unsigned char *barray;
  bool error;
  unsigned char *garray;
  long int height;
  int i;
  unsigned char *indexb;
  unsigned char *indexg;
  unsigned char *indexr;
  int j;
  int j2;
  int numbytes;
  unsigned char *rarray;
  unsigned long int width;

  width = 200;
  height = 200;
//
//  Allocate the memory.
//
  numbytes = width * abs ( height ) * sizeof ( unsigned char );  

  rarray = new unsigned char[numbytes];
  garray = new unsigned char[numbytes];
  barray = new unsigned char[numbytes];
//
//  Set the data.
//  Note that BMP files go from "bottom" up, so we'll reverse the
//  sense of "J" here to get what we want.
//
  indexr = rarray;
  indexg = garray;
  indexb = barray;

  for ( j2 = 0; j2 < abs ( height ); j2++ )
  {
    j = abs ( height ) - j2;
    for ( i = 0; i < width; i++ )
    {
      if ( i <= j )
      {
        *indexr = 255;
        *indexg = 0;
        *indexb = 0;
      }
      else if ( ( width - 1 ) * j + ( abs ( height ) - 1 ) * i <= 
        ( width - 1 ) * ( abs ( height ) - 1 ) )
      {
        *indexr = 0;
        *indexg = 255;
        *indexb = 0;
      }
      else
      {
        *indexr = 0;
        *indexg = 0;
        *indexb = 255;
      }
      indexr = indexr + 1;
      indexg = indexg + 1;
      indexb = indexb + 1;
    }
  }
//
//  Write the data to a file.
//
  error = bmp_24_write ( file_out_name, width, height, rarray, garray, barray );

  if ( error )
  {
    cout << "\n";
    cout << "BMP_24_WRITE_TEST - Fatal error!\n";
    cout << "  The test failed.\n";
    return error;
  }
//
//  Free the memory.
//
  delete [] rarray;
  delete [] garray;
  delete [] barray;

  if ( VERBOSE )
  {
    cout << "\n";
    cout << "BMP_24_WRITE_TEST:\n";
    cout << "  The test was successful.\n";
  }

  error = false;
  return error;
# undef VERBOSE
}
//****************************************************************************

bool long_int_read ( long int *long_int_val, ifstream &file_in )

//****************************************************************************
//
//
{
  bool error;
  unsigned short int u_short_int_val_hi;
  unsigned short int u_short_int_val_lo;

  if ( bmp_byte_swap )
  {
    error = u_short_int_read ( &u_short_int_val_lo, file_in );
    if ( error )
    {
      return error;
    }
    error = u_short_int_read ( &u_short_int_val_hi, file_in );
    if ( error )
    {
      return error;
    }
  }
  else
  {
    error = u_short_int_read ( &u_short_int_val_hi, file_in );
    if ( error )
    {
      return error;
    }
    error = u_short_int_read ( &u_short_int_val_lo, file_in );
    if ( error )
    {
      return error;
    }
  }

  *long_int_val = ( long int ) 
    ( u_short_int_val_hi << 16 ) | u_short_int_val_lo;

  return false;
}
//****************************************************************************

void long_int_write ( long int long_int_val, ofstream &file_out )

//****************************************************************************
//
{
  long int temp;
  unsigned short int u_short_int_val_hi;
  unsigned short int u_short_int_val_lo;

  temp = long_int_val / 65536;
  if ( temp < 0 )
  {
    temp = temp + 65536;
  }
  u_short_int_val_hi = ( unsigned short ) temp;

  temp = long_int_val % 65536;
  if ( temp < 0 )
  {
    temp = temp + 65536;
  }
  u_short_int_val_lo = ( unsigned short ) temp;

  if ( bmp_byte_swap )
  {
    u_short_int_write ( u_short_int_val_lo, file_out );
    u_short_int_write ( u_short_int_val_hi, file_out );
  }
  else
  {
    u_short_int_write ( u_short_int_val_hi, file_out );
    u_short_int_write ( u_short_int_val_lo, file_out );
  }

  return;
}
//****************************************************************************

bool u_long_int_read ( unsigned long int *u_long_int_val, 
  ifstream &file_in )

//****************************************************************************
//
{
  bool error;
  unsigned short int u_short_int_val_hi;
  unsigned short int u_short_int_val_lo;

  if ( bmp_byte_swap )
  {
    error = u_short_int_read ( &u_short_int_val_lo, file_in );
    if ( error )
    {
      return error;
    }
    error = u_short_int_read ( &u_short_int_val_hi, file_in );
    if ( error )
    {
      return error;
    }
  }
  else
  {
    error = u_short_int_read ( &u_short_int_val_hi, file_in );
    if ( error )
    {
      return error;
    }
    error = u_short_int_read ( &u_short_int_val_lo, file_in );
    if ( error )
    {
      return error;
    }
  }
//

  *u_long_int_val = ( u_short_int_val_hi << 16 ) | u_short_int_val_lo;

  return false;
}

void u_long_int_write ( unsigned long int u_long_int_val, 
  ofstream &file_out )

//
// 
//
{
  unsigned short int u_short_int_val_hi;
  unsigned short int u_short_int_val_lo;

  u_short_int_val_hi = ( unsigned short ) ( u_long_int_val / 65536 );
  u_short_int_val_lo = ( unsigned short ) ( u_long_int_val % 65536 );

  if ( bmp_byte_swap )
  {
    u_short_int_write ( u_short_int_val_lo, file_out );
    u_short_int_write ( u_short_int_val_hi, file_out );
  }
  else
  {
    u_short_int_write ( u_short_int_val_hi, file_out );
    u_short_int_write ( u_short_int_val_lo, file_out );
  }

  return;
}
//****************************************************************************

bool u_short_int_read ( unsigned short int *u_short_int_val, 
  ifstream &file_in )

//****************************************************************************
//
//  Purpose:
// 
//    U_SHORT_INT_READ reads an unsigned short int from a file.
//
//  Modified:
//
//    30 March 2005
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Output, unsigned short int *U_SHORT_INT_VAL, the value that was read.
//
//    Input, ifstream &FILE_IN, a reference to the input file.
//
//    Output, bool U_SHORT_INT_READ, is true if an error occurred.
//
{
  char c;
  unsigned char chi;
  unsigned char clo;

  if ( bmp_byte_swap )
    {
    file_in.read ( &c, 1 );
    if ( file_in.eof() )
    {
      return true;
    }
    clo = ( unsigned char ) c;

    file_in.read ( &c, 1 );
    if ( file_in.eof() )
    {
      return true;
    }
    chi = ( unsigned char ) c;
  }
  else
  {
    file_in.read ( &c, 1 );
    if ( file_in.eof() )
    {
      return true;
    }
    chi = ( unsigned char ) c;

    file_in.read ( &c, 1 );
    if ( file_in.eof() )
    {
      return true;
    }
    clo = ( unsigned char ) c;
  }

  *u_short_int_val = ( chi << 8 ) | clo;

  return false;
}
//****************************************************************************

void u_short_int_write ( unsigned short int u_short_int_val, 
  ofstream &file_out )

//****************************************************************************
//
//  Purpose:
// 
//    U_SHORT_INT_WRITE writes an unsigned short int to a file.
//
//  Modified:
//
//    26 February 2003
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    Input, unsigned short int *U_SHORT_INT_VAL, the value to be written.
//
//    Input, ofstream &FILE_OUT, a reference to the output file.
//
{
  unsigned char chi;
  unsigned char clo;

  chi = ( unsigned char ) ( u_short_int_val / 256 );
  clo = ( unsigned char ) ( u_short_int_val % 256 );

  if ( bmp_byte_swap )
  {
    file_out << clo << chi;
  }
  else
  {
    file_out << chi << clo;
  }

  return;
}
