/******************************************************************************
 * $Id: ogr_sxfdriver.cpp  $
 *
 * Project:  SXF Translator
 * Purpose:  Definition of classes for OGR SXF driver.
 * Author:   Ben Ahmed Daho Ali, bidandou(at)yahoo(dot)fr
 *           Dmitry Baryshnikov, polimax@mail.ru
 *
 ******************************************************************************
 * Copyright (c) 2011, Ben Ahmed Daho Ali
 * Copyright (c) 2013, NextGIS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

#include "cpl_conv.h"
#include "ogr_sxf.h"

CPL_CVSID("$Id: ogrsxfdriver.cpp  $");


extern "C" void RegisterOGRSXF();  

/************************************************************************/
/*                       ~OGRSXFDriver()                         */
/************************************************************************/

OGRSXFDriver::~OGRSXFDriver()
{
}

/************************************************************************/
/*                              GetName()                               */
/************************************************************************/

const char *OGRSXFDriver::GetName()

{
    return "SXF";
}

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

OGRDataSource *OGRSXFDriver::Open( const char * pszFilename, int bUpdate )

{
/* -------------------------------------------------------------------- */
/*      Determine what sort of object this is.                          */
/* -------------------------------------------------------------------- */

    VSIStatBufL sStatBuf;
    if (!EQUAL(CPLGetExtension(pszFilename), "sxf") ||
        VSIStatL(pszFilename, &sStatBuf) != 0 ||
        !VSI_ISREG(sStatBuf.st_mode))
        return FALSE;

    OGRSXFDataSource   *poDS = new OGRSXFDataSource();

    if( !poDS->Open( pszFilename, bUpdate ) )
    {
        delete poDS;
        poDS = NULL;
    }

    return poDS;
}

/************************************************************************/
/*                           DeleteDataSource()                         */
/************************************************************************/

OGRErr OGRSXFDriver::DeleteDataSource(const char* pszName)
{
    int iExt;
    //TODO: add more extensions if aplicable
    static const char *apszExtensions[] = { "szf", "rsc", NULL }; 

    VSIStatBufL sStatBuf;
    if (VSIStatL(pszName, &sStatBuf) != 0)
    {
        CPLError(CE_Failure, CPLE_AppDefined,
            "%s does not appear to be a valid sxf file.",
            pszName);

        return OGRERR_FAILURE;
    }

    for (iExt = 0; apszExtensions[iExt] != NULL; iExt++)
    {
        const char *pszFile = CPLResetExtension(pszName,
            apszExtensions[iExt]);
        if (VSIStatL(pszFile, &sStatBuf) == 0)
            VSIUnlink(pszFile);
    }

    return OGRERR_NONE;
}

/************************************************************************/
/*                           TestCapability()                           */
/************************************************************************/

int OGRSXFDriver::TestCapability( const char * pszCap )

{
    if (EQUAL(pszCap, ODrCDeleteDataSource))
        return TRUE;
    else
        return FALSE;
}

/************************************************************************/
/*                        RegisterOGRSXF()                       */
/************************************************************************/
void RegisterOGRSXF()
{
    OGRSFDriverRegistrar::GetRegistrar()->RegisterDriver( new OGRSXFDriver );   
}



