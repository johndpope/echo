#pragma once

#include <engine/core/Base/type_def.h>
#include <freeimage/FreeImage.h>
#include <engine/core/util/StringUtil.h>
#include <engine/core/math/color.h>

namespace Echo
{
	/**
	 * FreeImage���װ 2014-6-25
	 */
	class FreeImageHelper
	{
	public:
		/** ͼ����Ϣ */
		struct ImageInfo
		{
			ui32					m_width;		// ��
			ui32					m_height;		// ��
			FREE_IMAGE_COLOR_TYPE	m_colorType;	// ��ʽ����
		};

	public:
		FreeImageHelper();
		~FreeImageHelper();

		/** ��ȡͼ����Ϣ */
		static bool getImageInfo( const char* filePath, ImageInfo& imageInfo);

		/** ������������ */
		static bool rescaleImage( const char* iFilePath, const char* oFilePath, bool isResale=false, float scaleValue=1.f);

		/** ����Ϊtga��ʽ����ѹ�� */
		static bool saveImageToTGA( const char* iFilePath, const char* oFilePath, bool isResale=false, float scaleValue=1.f);

		/** ����Ϊbmp��ʽ����ѹ�� */
		static bool saveImageToBmp( const char* iFilePath, const char* oFilePath, bool isResale, ui32 width, ui32 height);

		/** ��ȡalphaͨ����һ�����ļ� */
		static bool extracRGBAlphaChannel( const char* srcPath, const char* oRgbFile, const char* oAlphaFile);

		/** ӳ���� */
		static int MappingFlagsByFormat( FREE_IMAGE_FORMAT fmt);

		/** ����Ϊbmp��ʽ(��֧��RGB��ʽ) */
		static int SaveImageToBmp( BYTE* pixelData, int widht, int height, const char* savePath);

		/** ��ȡͼƬ����buffer */
		static bool getImageBits(const char* filePath,unsigned char* bits);

		/** ���ˮӡ */
		static bool	addWaterMark(const char* dstFile, const char* srcFile);

		/** ��alphaͨ���ֽ⵽RGB����ӵ�ĩβ */
		static bool SaveRGBAtoRGBjpeg(const char* srcFile, const char* dstFile, ImageInfo& imageInfo,float scale);

		/** �ϲ���ͼ����߶����ӣ����tga�� */
		static bool mergeIMage(StringArray* srcFile, const char* dstFile);

		/** ���ļ�����ȡ��ɫֵ */
		static bool extractColors(const char* srcPath, std::vector<Echo::Color>& colors, int& width, int& height);
	};
}