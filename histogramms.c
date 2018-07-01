/*-------------------------------------------------------------------
 *                                                                   
 *                   -- People Detection  --
 *                                         
 * Authors:  Juan Alberto Rivera-Bautista
 *           & 
 *           Antonio Marin-Hernandez
 *
 *           Universidad Veracruzana
 *           Artificial Intelligence Department
 *           Xalapa, Mexico
 *
 *           2011-2012
 *
 *
 *------------------------------------------------------------------*/
#include <stdio.h>
#include <sys/time.h>
#include <opencv/cv.h>

/****************** Histogramms Variables ***************************/
int _binFactor = 1;
int _internalBins= 768;
int _maskValue=250;
int _reductFact=4;
/*************************************************************************/
// Funcion que obtiene el histograma de la region 3D de una persona
// Entradas:
//          ind: Indice de la persona en el arreglo de estructura Personas
/*************************************************************************/
int cvDiaGetMaskColorHistogram(IplImage *img,IplImage *mask, float *histo)
{
  int i,r,g,b;
  int size,N=0,maxBin;
  unsigned char *mData,*imgData;
  //  float *arrHist;
  // float arrHistAux[ 64 * 3 ] = {0};

  //  arrHist=(float*)malloc(_internalBins*sizeof(float));
  for (i=0;i<_internalBins;i++) histo[i]=0.0;

  size=img->width*img->height;
  mData=(unsigned char*)mask->imageData;
  imgData=(unsigned char*)img->imageData;
  for(i=0; i<size; i++ ){
    if (mData[i]>=_maskValue) {
      r = (int) imgData[i*3];
      g = (int) imgData[i*3+1];
      b = (int) imgData[i*3+2];
      // arrHist[r] += 1;
      histo[r/_reductFact]+= 1;
      // arrHist[g + 256] += 1;
      histo[g/_reductFact+64]+=1;
      //arrHist[b + 512] += 1;
      histo[b/_reductFact+128]+=1;
     N++;
    }
  }

  printf("N data %d\n",N);
  //Se redimensiona el histograma de 256*3 a 64*3
/*   r = 0; */
/*   for(i=0; i< _internalBins; i++ ){ */
/*     if( ( (i + 1) % 4 ) == 0 ) r++; */
/*     arrHistAux[ r ] = arrHistAux[ r ] + arrHist[ i ]; */
/*   } */
 
  //Histogramm Normalization
  for(i=0; i<64*3; i++ ){
    histo[i] = histo[i]/N; //arrHistAux[i]/N;
    printf ("->[%d]=%f\n",i,histo[i]);//,arrHistAux[i]/N);
  }
  return N;
}

/************************************************************************/
int cvDiaMaskColorHistogramm(IplImage *src,IplImage *mask,int histo)
{
  IplImage *colorMask;

  cvAnd( src, mask, colorMask, NULL );
  //  cvDisplay(colorMask);

  return 0;
}
