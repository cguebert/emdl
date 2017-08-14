#pragma once

#include <emdl/pdu/items/BaseItem.h>

namespace emdl
{
	namespace pdu
	{
		/// Asynchronous Operations Window Sub-Item (PS 3.7, D.3.3.3.1 and D.3.3.3.2).
		class EMDL_API AsynchronousOperationsWindow : public BaseItem
		{
		public:
			AsynchronousOperationsWindow(uint16_t maxNbOperationsInvoked,
										 uint16_t maxNbOperationsPerformed);
			AsynchronousOperationsWindow(std::istream& in);

			Field<uint16_t> maximumNumberOperationsInvoked;
			Field<uint16_t> maximumNumberOperationsPerformed;

		private:
			AsynchronousOperationsWindow();
		};
	}
}
