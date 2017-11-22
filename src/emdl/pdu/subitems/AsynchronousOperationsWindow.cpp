#include <emdl/pdu/subitems/AsynchronousOperationsWindow.h>
#include <emdl/pdu/items/ItemFactory.h>

namespace emdl
{
	namespace pdu
	{
		AsynchronousOperationsWindow::AsynchronousOperationsWindow()
			: maximumNumberOperationsInvoked(initField("Maximum-number-operations-invoked", 0))
			, maximumNumberOperationsPerformed(initField("Maximum-number-operations-performed", 0))
		{
			itemType.set(ItemType::AsynchronousOperationsWindow);
		}

		AsynchronousOperationsWindow::AsynchronousOperationsWindow(uint16_t maxNbOperationsInvoked,
																   uint16_t maxNbOperationsPerformed)
			: AsynchronousOperationsWindow()
		{
			maximumNumberOperationsInvoked.set(maxNbOperationsInvoked);
			maximumNumberOperationsPerformed.set(maxNbOperationsPerformed);
		}

		AsynchronousOperationsWindow::AsynchronousOperationsWindow(std::istream& in)
			: AsynchronousOperationsWindow()
		{
			read(in);
		}

		RegisterItem<AsynchronousOperationsWindow> asynchronousOperationsWindowSubItem{ItemType::AsynchronousOperationsWindow};
	}
}
