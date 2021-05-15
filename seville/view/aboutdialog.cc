#include "seville/view/aboutdialog.h"

namespace seville
{
   namespace view
   {
      AboutDialog::AboutDialog(QWidget* widget_parent_ptr)
         : QDialog(widget_parent_ptr)
      {
          do_init();
      }

      void AboutDialog::on_okButtonWasClickedEvent(void) {
         close();
      }

      AboutDialog::~AboutDialog(void)
      {
      }

      void AboutDialog::do_setupView(void) {
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         QVBoxLayout* layoutPtr = new QVBoxLayout(this);
         QLabel* aboutLabelPtr =
               new QLabel(QString("Seville v%1")
                          .arg(SevilleApp().kAppVersion), this);

         layoutPtr->addWidget(aboutLabelPtr);

         QPushButton* okButtonPtr = new QPushButton(tr("&OK"), this);
         layoutPtr->addWidget(okButtonPtr);
         setLayout(layoutPtr);

         connect(okButtonPtr, &QPushButton::clicked,
                 this, &seville::view::AboutDialog::on_okButtonWasClickedEvent);
      }

      void AboutDialog::do_init(void) {
         do_setupView();
      }
   }
}
