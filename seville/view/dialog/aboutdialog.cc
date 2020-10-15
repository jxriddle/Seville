#include "seville/view/dialog/aboutdialog.h"

namespace seville
{
   namespace view
   {
      namespace dialog
      {

         AboutDialog::AboutDialog(QWidget* parent_widget_ptr) \
            : QDialog(parent_widget_ptr)
         {
             do_init_();
         }

         auto AboutDialog::on_button_ok_clicked(void) -> void {
            close();
         }

         AboutDialog::~AboutDialog(void)
         {
         }

         auto AboutDialog::do_setup_view_(void) -> void {
            setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
            QVBoxLayout* layout_ptr = new QVBoxLayout(this);
            QLabel* label_about_ptr =
                  new QLabel(QString("Seville v%1")
                             .arg(Seville()->kAppVersion), this);

            layout_ptr->addWidget(label_about_ptr);

            QPushButton *button_ok_ptr = new QPushButton(tr("&OK"), this);
            layout_ptr->addWidget(button_ok_ptr);
            setLayout(layout_ptr);

            connect(button_ok_ptr, &QPushButton::clicked, \
                    this, &seville::view::dialog::AboutDialog::\
                    on_button_ok_clicked);
         }

         auto AboutDialog::do_init_(void) -> void {
            do_setup_view_();
         }
      }
   }
}
