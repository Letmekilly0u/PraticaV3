#include <iostream>
#include <FreeImage.h>
#include <string>
#include <algorithm>
#include <fstream>

// Класс для инициализации и уничтожения библиотеки автоматически
class FreeImageLibHolder
{
  public:
    FreeImageLibHolder()
    {
      FreeImage_Initialise();
      std::cout << "Lib is initialise.\n";
    }
    ~FreeImageLibHolder()
    {
      FreeImage_DeInitialise();
      std::cout << "Lib is deInitialise.\n";
    }
};

class IApcGraphicsHelper
{
public:
  //Создание изображения FreeImage (BMP формат)
  int createFreeImage(unsigned char *memory,
    size_t fileSize,
    FIBITMAP*& image);

  //Декодирование в JPEG
  int encodeImageToJPEG(unsigned char *memory,
    size_t fileSize);

  //Декодирование в JPEG с учетом размера
  int encodeImageToJPEG(unsigned char *memory,
    size_t fileSize,
    int anMaxWidth,
    int anMaxHeight);

  //Вращение изображения и декодирование в JPEG
  int rotateImageEncodeToJPEG(unsigned char *memory,
    size_t fileSize,
    double angle);

 //Вращение изображения (для функций)
  int rotateImage(FIBITMAP*& image,
    double angle);

  //Обоезает по краям изображение на заданное число пикселей
  int cutImageMiddle(unsigned char *memory,
    size_t fileSize,
    int pixelCount);

 //Обрезает изображение по зданным точкам
 int cutImagePoints(unsigned char *memory,
   size_t fileSize,
   const int pointLeft,
   const int pointTop,
   const int pointRight,
   const int pointBottomop);

//Получения размеров изображение (для функций)
int getImageSize(FIBITMAP*& image);

//Получение формата изображения
int getImageFormat(unsigned char *memory,
  size_t fileSize);

//Декодирование в BMP
int encodeImageToBMP(unsigned char *memory,
  size_t fileSize);


};
