#include <string.h>

#include "stm32h7/stm32h7_can.h"
#include "stm32h7xx_hal.h"

static can_receive_callback can_rcv_cb;
//static uint16_t RxBufferIdx = 0;
static FDCAN_HandleTypeDef *fdcan_handle;

bool can_init_stm(FDCAN_HandleTypeDef *handle,  can_receive_callback receive_callback) {
	//bind user callback function and fdcan handle
	can_rcv_cb = receive_callback;
	fdcan_handle = handle;

	//Enable new message interrupts on Rx FIFO0
	if(HAL_FDCAN_ActivateNotification(handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
	{
		//Error_Handler();
		//TODO: report error to log
	}

	return HAL_FDCAN_Start(handle)== HAL_OK;
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	//variables to store Rx data
	FDCAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];

	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
	{
		/* Retreive Rx messages from RX FIFO0 */
		if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
		{
			/* Reception Error */
			//Error_Handler();
			//TODO: handle reception error
		}

		/*if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK) {
		   Notification Error
		  Error_Handler();
		}*/
	}
	can_msg_t rcvd_msg;
	rcvd_msg.data_len = RxHeader.DataLength;
	rcvd_msg.sid = RxHeader.Identifier;
	memcpy(rcvd_msg.data, RxData, rcvd_msg.data_len);
	can_rcv_cb(&rcvd_msg, RxHeader.RxTimestamp);
}

bool can_send(const can_msg_t* message) {
	FDCAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8] = {0};

	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.MessageMarker = 0;
	TxHeader.Identifier = message->sid;

	memcpy(TxData, message->data, message->data_len);

	if (HAL_FDCAN_AddMessageToTxFifoQ(fdcan_handle, &TxHeader, TxData)!= HAL_OK)
	{
		return false;
	}
	return true;
}

bool can_send_rdy(void) {
	return HAL_FDCAN_GetTxFifoFreeLevel(fdcan_handle) > 0;
}


