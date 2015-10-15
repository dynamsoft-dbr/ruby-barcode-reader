#include <ruby.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <If_DBR.h>

#define strcmpi(dst, src) strcasecmp(dst, src)

__int64 GetFormat(const char * pstr)
{
	__int64 llFormat = 0;
	const char * pszFormat = pstr;

	if (strcasestr(pszFormat, "code_39") != NULL)
		llFormat |= CODE_39;
	if (strcasestr(pszFormat, "code_128") != NULL)
		llFormat |= CODE_128;
	if (strcasestr(pszFormat, "code_93") != NULL)
		llFormat |= CODE_93;
	if (strcasestr(pszFormat, "codabar") != NULL)
		llFormat |= CODABAR;
	if (strcasestr(pszFormat, "itf") != NULL)
		llFormat |= ITF;
	if (strcasestr(pszFormat, "upc_a") != NULL)
		llFormat |= UPC_A;
	if (strcasestr(pszFormat, "upc_e") != NULL)
		llFormat |= UPC_E;
	if (strcasestr(pszFormat, "ean_13") != NULL)
		llFormat |= EAN_13;
	if (strcasestr(pszFormat, "ean_8") != NULL)
		llFormat |= EAN_8;
	if (strcasestr(pszFormat, "industrial_25") != NULL)
		llFormat |= INDUSTRIAL_25;
	if (strcasestr(pszFormat, "oned") != NULL)
		llFormat = OneD;
	if (strcasestr(pszFormat, "qr_code") != NULL)
		llFormat |= QR_CODE;


	return llFormat;
}

const char * GetFormatStr(__int64 format)
{
	if (format == CODE_39)
		return "CODE_39";
	if (format == CODE_128)
		return "CODE_128";
	if (format == CODE_93)
		return "CODE_93";
	if (format == CODABAR)
		return "CODABAR";
	if (format == ITF)
		return "ITF";
	if (format == UPC_A)
		return "UPC_A";
	if (format == UPC_E)
		return "UPC_E";
	if (format == EAN_13)
		return "EAN_13";
	if (format == EAN_8)
		return "EAN_8";
	if (format == INDUSTRIAL_25)
		return "INDUSTRIAL_25";
	if (format == QR_CODE)
		return "QR_CODE";

	return "UNKNOWN";
}

VALUE decode(const char *pszImageFile)
{
	VALUE ary = rb_ary_new();

	__int64 llFormat = (OneD |QR_CODE);
	int iMaxCount = 0x7FFFFFFF;
	int iIndex = 0;
	ReaderOptions ro = {0};
	pBarcodeResultArray paryResult = NULL;
	int iRet = -1;
	char * pszTemp = NULL;
	char * pszTemp1 = NULL;
	struct timeval begin, end;

	if (NULL == pszImageFile)
	{
		printf("The syntax of the command is incorrect.\n");
		return 1;
	}

	// Set license
	DBR_InitLicense("B8DF4560D0953299D5454C71A38D1FEB");

	// Read barcode
	gettimeofday(&begin, NULL);
	ro.llBarcodeFormat = llFormat;
	ro.iMaxBarcodesNumPerPage = iMaxCount;
	iRet = DBR_DecodeFile(pszImageFile, &ro, &paryResult);
	gettimeofday(&end, NULL);

	// Output barcode result
	pszTemp = (char*)malloc(4096);
	if (iRet != DBR_OK)
	{
		sprintf(pszTemp, "Failed to read barcode: %s\r\n", DBR_GetErrorString(iRet));
		printf("%s", pszTemp);
		free(pszTemp);
		return 1;
	}

	if (paryResult->iBarcodeCount == 0)
	{
		sprintf(pszTemp, "No barcode found. Total time spent: %.3f seconds.\r\n",
					((float)((end.tv_sec * 1000 * 1000 +  end.tv_usec) - (begin.tv_sec * 1000 * 1000 + begin.tv_usec))/(1000 * 1000)));
		printf("%s", pszTemp);
		DBR_FreeBarcodeResults(&paryResult);
		return 0;
	}

	sprintf(pszTemp, "Total barcode(s) found: %d. Total time spent: %.3f seconds\r\n\r\n", paryResult->iBarcodeCount,
					((float)((end.tv_sec * 1000 * 1000 +  end.tv_usec) - (begin.tv_sec * 1000 * 1000 + begin.tv_usec))/(1000 * 1000)));
	printf("%s", pszTemp);
	for (iIndex = 0; iIndex < paryResult->iBarcodeCount; iIndex++)
	{
		sprintf(pszTemp, "Barcode %d:\r\n", iIndex + 1);
		// printf("%s", pszTemp);
		sprintf(pszTemp, "%s    Page: %d\r\n", pszTemp, paryResult->ppBarcodes[iIndex]->iPageNum);
		// printf("%s", pszTemp);
		sprintf(pszTemp, "%s    Type: %s\r\n", pszTemp, GetFormatStr(paryResult->ppBarcodes[iIndex]->llFormat));
		// printf("%s", pszTemp);
		pszTemp1 = (char*)malloc(paryResult->ppBarcodes[iIndex]->iBarcodeDataLength + 1);
		memset(pszTemp1, 0, paryResult->ppBarcodes[iIndex]->iBarcodeDataLength + 1);
		memcpy(pszTemp1, paryResult->ppBarcodes[iIndex]->pBarcodeData, paryResult->ppBarcodes[iIndex]->iBarcodeDataLength);
		sprintf(pszTemp, "%s    Value: %s\r\n", pszTemp, pszTemp1);
		// printf("%s", pszTemp);
		free(pszTemp1);
		// sprintf(pszTemp, "    Region: {Left: %d, Top: %d, Width: %d, Height: %d}\r\n\r\n",
		// 	paryResult->ppBarcodes[iIndex]->iLeft, paryResult->ppBarcodes[iIndex]->iTop,
		// 	paryResult->ppBarcodes[iIndex]->iWidth, paryResult->ppBarcodes[iIndex]->iHeight);
		// printf("%s", pszTemp);
		rb_ary_push(ary, rb_str_new_cstr(pszTemp));
	}

	free(pszTemp);
	DBR_FreeBarcodeResults(&paryResult);

	return ary;
}

VALUE BarcodeReader = Qnil;

void Init_dynamsoftbarcode();

VALUE method_decodeFile(VALUE self, VALUE path);

void Init_dynamsoftbarcode() {
	BarcodeReader = rb_define_module("BarcodeReader");
	rb_define_method(BarcodeReader, "decodeFile", method_decodeFile, 1);
}

VALUE method_decodeFile(VALUE self, VALUE path) {
	char *pszPath = StringValueCStr(path);
	VALUE ary = decode(pszPath);
	return ary;
}
