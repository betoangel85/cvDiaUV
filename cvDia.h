/********************************************************************
 *                   cvDia- OpenCV DIA-UV funcions
 *
 *                           writed by :
 *
 *                    Juan Alberto Rivera-Bautista
 *                          Arturo Cruz Maya
 *                      Antonio Marin-Hernandez
 *
 *                    Last version March 2013
 *                                                                 
 *                Deparment of Artificial Intelligence
 *                      University of Veracruz
 *                                                                  
 * cvDia.h is free to use and modify for any purpose, with no     
 * restrictions of copyright or license.                            
 ********************************************************************/

#ifndef _CV_DIAUV_H
#define _CV_DIAUV_H


#ifdef __cplusplus
extern "C" {
#endif

  /* Histogramms */
  extern int cvDiaGetMaskColorHistogram(IplImage *img,IplImage *mask, float *histo);


  /*People Detection */
  extern int cvDiaInitPeopleDet(int minFSize,int maxFSize,char *fileName);

  extern int cvDiaFindFaces(IplImage *frame, int *facesFound,
		     CvRect **bBox,CvRect **noFacesBox);
  extern int cvDiaSkinColorDetect( IplImage *frame, CvRect *r );  

  extern int cvDiaFindEyes(IplImage *frame, int *eyesFound,
			    CvRect **bBox);


#ifdef __cplusplus
}
#endif


#endif /* _CV_DIAUV_H */

