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

      auto AboutDialog::on_okButtonWasClickedEvent(void) -> void {
         close();
      }

      AboutDialog::~AboutDialog(void)
      {
      }

      auto AboutDialog::do_setupView(void) -> void {
         setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
         QVBoxLayout* layout_ptr = new QVBoxLayout(this);
         QLabel* label_about_ptr =
               new QLabel(QString("Seville v%1")
                          .arg(SevilleApp().kAppVersion), this);

         layout_ptr->addWidget(label_about_ptr);

         QPushButton* button_ok_ptr = new QPushButton(tr("&OK"), this);
         layout_ptr->addWidget(button_ok_ptr);
         setLayout(layout_ptr);

         connect(button_ok_ptr, &QPushButton::clicked,
                 this, &seville::view::AboutDialog::on_okButtonWasClickedEvent);
      }

      auto AboutDialog::do_init(void) -> void {
         do_setupView();
      }
   }
}
