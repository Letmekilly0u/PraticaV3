#include "ObertkaHeader.h"

// Функция загрузки изображения из памяти
int IApcGraphicsHelper::createFreeImage(unsigned char *memory, size_t fileSize, FIBITMAP*& image)
{
  //Проверка на корректность входных данных
  if (memory == NULL || fileSize == 0){
      std::cout << "creatFreeImage: couldn't find the file in memory.\n";
      return -1;
  }

  //Открытие потока для загрузки из памяти устройства
  FIMEMORY* fiBuffer = FreeImage_OpenMemory(memory, fileSize);
  if (fiBuffer == nullptr){
      std::cout << "creatFreeImage(): couldn't load image from ofBuffer, opening FreeImage memory failed.\n";
      return -1;
  }

  //Опредеоение формата файла из памяти устройства
  FREE_IMAGE_FORMAT free_img_format = FreeImage_GetFileTypeFromMemory(fiBuffer);
  if (free_img_format == -1){
      std::cout << "creatFreeImage(): couldn't load image from ofBuffer, unable to guess image format from memory";
      return -1;
  }

  //Создание FreeImage изображения
  image = FreeImage_LoadFromMemory(free_img_format, fiBuffer, 0);
  if(image == NULL){
    std::cout << "creatFreeImage(): function is failed, error loading from memory.\n";
    return -1;
  }

  //Закрытие потока для работы с памятью
  FreeImage_CloseMemory(fiBuffer);
  if (fiBuffer != nullptr)
    return 0;
  else {
    std::cout << "creatFreeImage(): WARNING! The stream is not closed.\n";
    return -1;
  }
}

//Декодирование в JPEG
int IApcGraphicsHelper::encodeImageToJPEG(unsigned char *memory, size_t fileSize)
{
  FIBITMAP* image;
  createFreeImage(memory, fileSize, image);
  if (image == NULL){
    std::cout << "encodeImageToJPEG(): function is failed, failed to create image.\n";
    return -1;
  }
  FreeImage_Save(FIF_JPEG, image, "clone.jpeg", 0);
  FreeImage_Unload(image);
  if(image != NULL){
    std::cout << "encodeImageToJPEG(): WARNING! The image has not been uploaded.\n";
    return -1;
  }
    return 0;
}

//Декодирование в JPEG с учетом размера
int IApcGraphicsHelper::encodeImageToJPEG(unsigned char *memory,
  size_t fileSize,
  int anMaxWidth,
  int anMaxHeight)
{
  FIBITMAP* image;
  createFreeImage(memory, fileSize, image);
  if (image == NULL){
    std::cout << "encodeImageToJPEG(): function is failed, failed to create image.\n";
    return -1;
  }

  if (!FreeImage_GetWidth(image) || !FreeImage_GetHeight(image)){
    std::cout << "encodeImageToJPEG(): function is failed, the width or height has an incorrect size.\n";
    return -1;
  }

  if(!anMaxWidth || !anMaxHeight) {
    std::cout << "Max width: " << anMaxWidth << "or max height: " << anMaxHeight << "can't be return!";
    return -1;}

  int nImageWidth = FreeImage_GetWidth(image);
  int nImageHeight = FreeImage_GetHeight(image);

  if(!nImageWidth || !nImageHeight) {
    std::cout << "Max width: " << nImageWidth << "or max height: " << nImageHeight << "can't be return!";
    return -1;}

  double dblFirst = (double)anMaxWidth / (double)anMaxHeight;
  double dblSecond = (double)nImageWidth / (double)nImageHeight;

  //Если первое отношение меньше второго - режем по ширене
  std::string nTransformType = "";
  if(dblFirst < dblSecond)
  {
    nTransformType = "TRansformType_ProportionalWidth";
  }
  else
  {
    nTransformType = "TRansformType_ProportionalHeight";
  }

  if(nTransformType == "") {
    std::cout << "encodeImageToJPEG(): function is failed, error of execution.\n";
    return -1;}

  int nNewWidth = 0;
  int nNewHeigth = 0;

  if(nTransformType == "TRansformType_ProportionalWidth")
  {
    nNewWidth = anMaxWidth;
    nNewHeigth = int(double(FreeImage_GetHeight(image)) * double(anMaxWidth) / double(FreeImage_GetWidth(image)));
  }
  else
  {
    nNewHeigth = anMaxHeight;
    nNewWidth = int(double(FreeImage_GetWidth(image)) * double(anMaxHeight) / double(FreeImage_GetHeight(image)));
  }

  if (nNewWidth == 0 || nNewHeigth == 0){
    std::cout << "encodeImageToJPEG(): function is failed, error of execution.\n";
    return -1;
  }
  image = FreeImage_Rescale(image, nNewWidth, nNewHeigth, FILTER_BOX);

  FreeImage_Save(FIF_JPEG, image, "compressImg.jpeg", 0);
  FreeImage_Unload(image);
  if(image != NULL){
    std::cout << "encodeImageToJPEG(): WARNING! The image has not been uploaded.\n";
    return -1;
  }
  else{
    return 0;
  }
  return 0;
}

//Вращение изображения и декодирование в JPEG
int IApcGraphicsHelper::rotateImageEncodeToJPEG(unsigned char *memory,
  size_t fileSize,
  double angle)
{
  FIBITMAP* image;
  createFreeImage(memory, fileSize, image);

  image = FreeImage_Rotate(image, angle);

  FreeImage_Save(FIF_JPEG, image, "rotateImg.jpeg", 0);
  FreeImage_Unload(image);

  return 0;
}

//Вращение изображения (для функций)
int IApcGraphicsHelper::rotateImage(FIBITMAP*& image,
  double angle)
{
  image = FreeImage_Rotate(image, angle);
  return 0;
}

//Обоезает по краям изображение на заданное число пикселей
int IApcGraphicsHelper::cutImageMiddle(unsigned char *memory,
  size_t fileSize,
  int pixelCount)
{
  FIBITMAP* image;
  createFreeImage(memory, fileSize, image);
  if (image == NULL){
    std::cout << "cutImageMiddle(): function is failed, failed to create image.\n";
    return -1;
  }

  int nImageWidth = FreeImage_GetWidth(image);
  int nImageHeight = FreeImage_GetHeight(image);
  if(!nImageWidth || !nImageHeight) {
    std::cout << "Max width: " << nImageWidth << "or max height: " << nImageHeight << "can't be return!";
    return -1;}

  //Создаем точки для обрезки изображения
  int pointLeft = pixelCount;
  int pointRight = nImageWidth - pixelCount;
  int pointTop = pixelCount;
  int pointBottom = nImageHeight - pixelCount;
  if(pointTop == 0 || pointLeft == 0 || pointRight == 0 || pointBottom == 0){
    std::cout << "cutImageMiddle(): function is failed, error of execution.\n";
    return -1;
  }

  image = FreeImage_Copy(image, pointLeft, pointTop, pointRight, pointBottom);

  FreeImage_Save(FIF_JPEG, image, "cutImg.jpeg", 0);
  FreeImage_Unload(image);
  if(image != nullptr){
    std::cout << "encodeImageToJPEG(): WARNING! The image has not been uploaded.\n";
    return -1;
  }
  else{
    return 0;
  }
}

//Обрезает изображение по зданным точкам
int IApcGraphicsHelper::cutImagePoints(unsigned char *memory,
  size_t fileSize,
  const int pointLeft,
  const int pointTop,
  const int pointRight,
  const int pointBottom)
{
  FIBITMAP* image;
  createFreeImage(memory, fileSize, image);
  if (image == NULL){
    std::cout << "cutImageMiddle(): function is failed, failed to create image.\n";
    return -1;
  }

  if(pointTop == 0 || pointLeft == 0 || pointRight == 0 || pointBottom == 0){
    std::cout << "cutImageMiddle(): function is failed, uncorrected poits.\n";
    return -1;
  }

  int nImageWidth = FreeImage_GetWidth(image);
  int nImageHeight = FreeImage_GetHeight(image);
  if(!nImageWidth || !nImageHeight) {
    std::cout << "Max width: " << nImageWidth << "or max height: " << nImageHeight << "can't be return!";
    return -1;}

  getImageSize(image);
  image = FreeImage_Copy(image, pointLeft, pointTop, pointRight, pointBottom);

  FreeImage_Save(FIF_JPEG, image, "cutImg.jpeg", 0);
  FreeImage_Unload(image);
  if(image != nullptr){
    std::cout << "encodeImageToJPEG(): WARNING! The image has not been uploaded.\n";
    return -1;
  }
  else{
    return 0;
  }
}

//Получения размеров изображение (для функций)
int IApcGraphicsHelper::getImageSize(FIBITMAP*& image)
{
  int nImageWidth = FreeImage_GetWidth(image);
  int nImageHeight = FreeImage_GetHeight(image);
  if(!nImageWidth || !nImageHeight)
  {
    std::cout << "Image width: " << nImageWidth << "or image height: " << nImageHeight << "can't be return!";
    return -1;}
  else
  {
  std::cout << "Image size (width x height): " << nImageWidth << " x " << nImageHeight << ".\n";
  return 0;
  }
}


int IApcGraphicsHelper::getImageFormat(unsigned char *memory,
  size_t fileSize)
{
  //Проверка на корректность входных данных
  if (memory == NULL || fileSize == 0){
      std::cout << "creatFreeImage: couldn't find the file in memory.\n";
      return -1;
  }

  //Открытие потока для загрузки из памяти устройства
  FIMEMORY* fiBuffer = FreeImage_OpenMemory(memory, fileSize);
  if (fiBuffer == nullptr){
      std::cout << "creatFreeImage(): couldn't load image from ofBuffer, opening FreeImage memory failed.\n";
      return -1;
  }

  //Опредеоение формата файла из памяти устройства
  FREE_IMAGE_FORMAT free_img_format = FreeImage_GetFileTypeFromMemory(fiBuffer);
  if (free_img_format == -1){
      std::cout << "creatFreeImage(): couldn't load image from ofBuffer, unable to guess image format from memory";
      return -1;
  }

  //Закрытие потока для работы с памятью
  FreeImage_CloseMemory(fiBuffer);
  if (fiBuffer != nullptr)
  {
    std::cout << "File type: " << FreeImage_GetFormatFromFIF(free_img_format) << ".\n";
    return 0;
  }
  else {
    std::cout << "creatFreeImage(): WARNING! The stream is not closed.\n";
    return -1;
  }
}

//Декодирование в BMP
int IApcGraphicsHelper::encodeImageToBMP(unsigned char *memory,
  size_t fileSize)
{
  FIBITMAP* image;
  createFreeImage(memory, fileSize, image);
  if (image == NULL){
    std::cout << "cutImageMiddle(): function is failed, failed to create image.\n";
    return -1;
  }

  FreeImage_Save(FIF_BMP, image, "encodeBMP.bmp", 0);
  FreeImage_Unload(image);
  if(image != NULL){
    std::cout << "encodeImageToJPEG(): WARNING! The image has not been uploaded.\n";
    return -1;
  }
  return 0;
}
