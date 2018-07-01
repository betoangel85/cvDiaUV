
/*-------------------------------------------------------------------
 *                                                                   
 *                   -- People Detection  --
 *                                         
 * Authors:  Juan Alberto Rivera-Bautista,
 *           Arturo Cruz-Maya
 *           & 
 *           Antonio Marin-Hernandez
 *
 *           Universidad Veracruzana
 *           Artificial Intelligence Department
 *           Xalapa, Mexico
 *
 *           2011-2013
 *
 *
 *------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/time.h>
#include <opencv/cv.h>

/******************People detection Variables ***********************/
#define MaxPeople 3
#define minFaceTh 5

int porcRostro = 40;  //Porcentaje de piel en rostro
int umbPielMin = 50;  //Puntos mínimos del blob para considerarlo piel
int umbPielMax = 15000; //Puntos máximos del blob para considerarlo piel
int frameBuscaRostro = 20; //Frame en el que buscará rostro en la imagen
CvSize minFaceSize,maxFaceSize;

/***************VARIABLES ROSTRO/PIEL********************/
CvHaarClassifierCascade *cascade;
CvHaarClassifierCascade *cascadeEyes;
//CvCascadeClassifier cascade;
CvMemStorage *storage; 
CvMemStorage *storageEyes;  
//OpenCV file to detect faces 
char haarFilename[128];
char haarFilenameEyes[128];

/******************************************** ***********************/
int cvDiaInitPeopleDet(int minFSize,int maxFSize,char *fileName)
{
  if (minFSize>minFaceTh)
    minFaceSize=cvSize(minFSize,minFSize);
  else return -1;

  if (maxFSize>minFSize) maxFaceSize=cvSize(maxFSize,maxFSize);
  else return -1;

  if (fileName==NULL) 
    sprintf(haarFilename,"../haarcascade_frontalface_alt.xml");
  else {
    sprintf(haarFilename,"%s",fileName);
  }
  printf("Haar fileName %s\n",fileName);

  cascade = (CvHaarClassifierCascade*)cvLoad(haarFilename, 0,NULL,NULL);
  storage = cvCreateMemStorage(0);

  if (!cascade){ 
    fprintf(stderr,"OpenCV ERROR cascades file not found \n");
    exit (0);
  }

  return 0;
}


/************************************************************************/
// Calcula el histograma de una imagen/region 
// Entrada:
//         skin: Imagen de un solo canal
// Salida:
//        int: Bandera positiva si la region tiene piel arriba de un umbral
/************************************************************************/
int _getSkinColorPercent( IplImage *skin )
{
  int i,j,size;
  int pixB, totP, porE;
  float porc;
  int band;
   
  CvScalar pixel;
  pixB = 0;

  size =skin->width*skin->height;
  //Recorre la imagen/region para generar histograma
/*   for( i = 0; i < skin->width; i++ ){ */
/*     for( j = 0; j < skin->height; j++ ){ */
/*       pixel = cvGet2D( skin, j, i ); */
/*       if( pixel.val[0] == 255 ) pixB++; */
/*     } */
/*   } */

/*   for(i=0;i<size;i++) */
/*     if (skin->imageData[i]==255) pixB++; */

  pixB=cvCountNonZero(skin);


  //Calcula total de pixeles de la region
  totP = skin->width*skin->height;
  //Obtiene el % de pixeles de piel/blancos
  porc = (float)pixB/totP;
  porE = porc*100;
  //Si el % esta arriba de un umbral, la bandera es positiva
  if( porE >= porcRostro ){
    band = 1;
  }else{
    band = 0;
  }
  return band;
}



/***********************************************************************/
// Funcion que detecta piel en una region
// Entrada:
//         frame: Imagen RGB
//         r: region del rostro
// Salida:
//        int: Bandera si la region es rostro o no
/***********************************************************************/
int cvDiaSkinColorDetect( IplImage *frame, CvRect *r )
{   
  int band;
  IplImage *frameAux;
  IplImage *imagenYCrCb,*skinD;
  IplImage *planoY,*planoCr, *planoCb;
  IplImage *planoCr1, *planoCr2,*planoCb1,*planoCb2;
  CvRect rAux;

  frameAux= cvCreateImage( cvGetSize( frame ), 8, 3 );   
  cvCopy( frame, frameAux, NULL );
  // frameAux=frame;
  
  rAux = cvRect( r->x, r->y, r->width, r->height );
  //Limita la imagen total a solo la region de entrada
  cvSetImageROI( frameAux, rAux );

  //  cvSetImageROI( frameAux, *r );
   
  imagenYCrCb = cvCreateImage( cvGetSize(frameAux), 8, 3 );
  skinD = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  cvCvtColor( frameAux, imagenYCrCb, CV_RGB2YCrCb );

  planoY = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  planoCr = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  planoCb = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  cvCvtPixToPlane( imagenYCrCb, planoY, planoCr, planoCb, 0 );
  planoCr1 = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  planoCr2 = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  planoCb1 = cvCreateImage( cvGetSize(frameAux), 8, 1 );
  planoCb2 = cvCreateImage( cvGetSize(frameAux), 8, 1 );

  cvThreshold( planoCr, planoCr1, 133, UCHAR_MAX, CV_THRESH_BINARY );
  cvThreshold( planoCr, planoCr2, 173, UCHAR_MAX, CV_THRESH_BINARY_INV );
  cvThreshold( planoCb, planoCb1, 77, UCHAR_MAX, CV_THRESH_BINARY );
  cvThreshold( planoCb, planoCb2, 127, UCHAR_MAX, CV_THRESH_BINARY_INV );

  cvAnd( planoCr1, planoCr2, planoCr, NULL );
  cvAnd( planoCb1, planoCb2, planoCb, NULL );  
  cvAnd( planoCr, planoCb, skinD , NULL); 
   
      //Manda a llamar funcion que calcula el porcentaje de piel
  band = _getSkinColorPercent(skinD);
   
  cvReleaseImage(&frameAux);
  cvReleaseImage(&imagenYCrCb);
  cvReleaseImage(&skinD);
  cvReleaseImage(&planoY);
  cvReleaseImage(&planoCr);
  cvReleaseImage(&planoCb);
  cvReleaseImage(&planoCr1);
  cvReleaseImage(&planoCr2);
  cvReleaseImage(&planoCb1);
  cvReleaseImage(&planoCb2);
      
  return band;
}

/************************************************************************
 *cvDiaFindFaces -> This function find valid faces in a givern image
 *                  Faces found by Haar Cascade like features are
 *                  validated by a skin color threshold method
 *
 *                  cvDiaPeopleDet() should be called before call this
 *                  function
 *
 *Parameters IplImage *frame -> Image frame
 *           int *facesFound -> returns No of faces found in frame
 *	     CvRect **bBox   -> Bounding Boxes of Valid Faces 
 *           CvRect **noFacesBox-> Bounding Boxes of Not Validated Faces 
 *
 * returns int  >0 been the No. of valid faces fund 
 *              =0 if not valid faces found
 *              <0 Error           
 ************************************************************************/
int cvDiaFindFaces(IplImage *frame, int *facesFound,
	      CvRect **bBox,CvRect **noFacesBox)
{
  int val =0, i, ind = 0, noInd=0,NoFaces;
  CvRect *faceBox;
  CvRect *validFaces,*notValidFaces;
  CvSeq *faces;
  
  //Search for faces by Haar Like Features 
  faces=cvHaarDetectObjects(frame,cascade,storage,1.1,3,
			    CV_HAAR_DO_CANNY_PRUNING,
			    minFaceSize,maxFaceSize);  
  NoFaces=faces->total;

  if (NoFaces>0){
    //Memory allocation for CvRect structures 
    validFaces = (CvRect*)malloc(NoFaces*sizeof(CvRect));
    notValidFaces =(CvRect*)malloc(NoFaces*sizeof(CvRect));

    for(i=0; i<NoFaces; i++){
      faceBox=(CvRect *)cvGetSeqElem(faces, i);
      val=cvDiaSkinColorDetect(frame, faceBox);

      if( val == 1 ){
	validFaces[ind]=cvRect(faceBox->x, faceBox->y, 
			       faceBox->width, faceBox->height); 
	ind++;
      } else {
	notValidFaces[noInd]=cvRect(faceBox->x, faceBox->y, 
				  faceBox->width, faceBox->height); 
	//	printf("x y %d %d %d %d\n",faceBox->x, faceBox->y,
	//			  faceBox->width, faceBox->height);
	noInd++;
      }
    }

    *bBox = validFaces;
    *noFacesBox= notValidFaces;
    }
  
  *facesFound=NoFaces;
  return ind;
}

/************************************************************************
*cvDiaFindEyes -> This function find valid eyes in a given image
 *                  Eyes found by Haar Cascade like features 
 *                  
 *
 *Parameters IplImage *frame -> Image frame
 *           int *eyesFound -> returns No of faces found in frame
 *	     CvRect **bBox   -> Bounding Boxes of Valid Faces 
 *
 * returns int  >0 been the No. of valid eyes fund 
 *              =0 if not valid eyes found
 *              <0 Error           
 ************************************************************************/ 
int cvDiaFindEyes(IplImage *frame, int *eyesFound,
	      CvRect **bBox)
{
  int val =0, i, ind = 0,NoEyes;
  CvRect *eyesBox;
  CvRect *validEyes;
  CvSeq *eyes;
  
  //Search for eyes by Haar Like Features 
  eyes=cvHaarDetectObjects(frame,cascadeEyes,storageEyes,1.1,3,
			    CV_HAAR_DO_CANNY_PRUNING,
			    cvSize(10,10),cvSize(30,30));  
  NoEyes=eyes->total;

  if (NoEyes>0){
    //Memory allocation for CvRect structures 
    validEyes = (CvRect*)malloc(NoEyes*sizeof(CvRect));

    for(i=0; i<NoEyes; i++){
      eyesBox=(CvRect *)cvGetSeqElem(eyes, i);      
	validEyes[ind]=cvRect(eyesBox->x, eyesBox->y, 
			       eyesBox->width, eyesBox->height); 
	ind++;
      
    }

    *bBox = validEyes;    
    }
  
  *eyesFound=NoEyes;
  return ind;
}



/************************************************************************/
/* int cvDiaMaskColorHistogramm(IplImage *src,IplImage *mask,int histo) */
/* { */
/*   IplImage *colorMask; */

/*   cvAnd( src, mask, colorMask, NULL ); */
/*   //  cvDisplay(colorMask); */

/*   return 0; */
/* } */

