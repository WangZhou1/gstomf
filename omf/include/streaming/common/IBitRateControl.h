//
// Created by jg.wang on 2019/7/5.
//
#pragma once

#include "OmfBRCMode.h"
#include <vector>

namespace omf {
	namespace api {
		namespace streaming {
			namespace common {
				class IBitRateControl {
				public:
					typedef struct{const char* mode;const char* note;}mode_s;
					virtual bool SetBrcMode(const char*)=0;
					virtual std::vector<mode_s> GetBrcModes()const = 0;
					virtual mode_s GetBrcMode()const = 0;
				public:
					virtual std::vector<BRCMode> GetSupportedBRCModes()const = 0;
					virtual bool IsSupportBRCMode(BRCMode)const=0;
					virtual BRCMode GetCurrentBRCMode()const = 0;
					virtual bool SetBRCMode(BRCMode)=0;

				public:
					virtual bool IsSupportBitRateControl()const = 0;
					virtual bool IsSupportBitRateRange()const = 0;

					/**
				 * set the target bit rate for codec.
				 * @param br[in] bit per seconds.
				 * @note supported BitRateControl:\n
				 * 		CBRC:\n
				 * 		MBRC:\n
				 * 		DBRC:\n
				 * @see BitRateControl
				 */
					virtual bool SetBitRate(int br)=0;

					/**
					 * set the minimum bit rate(BR).
					 * @param min [in]the minimum
					 * @note supported BitRateControl:\n
					 * 	 	ABRC:\n
					 * @see BitRateControl
					 */
					virtual bool SetBitRateMinimum(int min)=0;

					/**
					 * set the maximum bit rate(BR).
					 * @param max [in]the maximum
					 * @note supported BitRateControl:\n
					 * 	 	ABRC:\n
					 * @see BitRateControl
					 */
					virtual bool SetBitRateMaximum(int max)=0;
					/**
					 * set vbrc grads flag.
					 * @param grads flag
					 * @see BitRateControl
					 */
					virtual bool SetVbrcGrads(bool flag)=0;

				public:
					virtual bool IsSupportQPControl()const = 0;
					virtual bool IsSupportQPRange()const = 0;

					/**
					 * manual set the quantization parameter.
					 * @param qp [in] quantization parameter. \n
					 * 		min: 10 \n
					 * 		max: 50 \n
					 * @note supported BitRateControl:\n
					 * 	 	MQPC: \n
					 * @see BitRateControl
					 */
					virtual bool SetQP(int qp)=0;

					/**
					 * set the minimum quantization parameter(QP).
					 * @param min [in]the minimum
					 * @note supported BitRateControl:\n
					 * 	 	MBRC:\n
					 * 	 	DBRC:\n
					 * 	 	ABRC:\n
					 * @see BitRateControl
					 */
					virtual bool SetQPMinimum(int min)=0;

					/**
					 * set the maximum quantization parameter(QP).
					 * @param max [in]the maximum
					 * @note supported BitRateControl:\n
					 * 	 	MBRC:\n
					 * 	 	DBRC:\n
					 * 	 	ABRC:\n
					 * @see BitRateControl
					 */
					virtual bool SetQPMaximum(int max)=0;
					/**
				 * set H264 Go num.
				 * @param gop num
				 * @note supported BitRateControl:\n
				 * @see BitRateControl
				 */
					virtual bool SetGopNum(int num)=0;

				public:
					/**
					 * get current bit per seconds.
					 * @return br.
					 */
					virtual int GetCurrentBitRate() const = 0;

					/**
					 * get current quantization parameter(QP).
					 * @return QP.
					 */
					virtual int GetCurrentQP() const = 0;
				};
			}
		}
	}
}