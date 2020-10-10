#include "user.h"

namespace seville
{
   namespace palace
   {
      void User::do_reset(void)
      {
         my_color_ = 1;
         my_face_ = 1;
         my_x_ = 0;
         my_y_ = 0;
         my_id_changed_ = false;
         my_id_counter_ = 0xf5dc385e;
         my_id_crc_ = 0xc144c580;
         my_reg_counter_ = 0xcf07309c;
         my_reg_crc_ = 0x5905f923;
         my_username_ = "Seville User";
         my_wizpass_ = "";
      }

      void User::do_assign(const User& user)
      {
         my_color_ = user.my_color_;
         my_face_ = user.my_face_;
         my_x_ = user.my_x_;
         my_y_ = user.my_y_;
         my_id_changed_ = user.my_id_changed_;
         my_id_counter_ = user.my_id_counter_;
         my_id_crc_ = user.my_id_crc_;
         my_reg_counter_ = user.my_reg_counter_;
         my_reg_crc_ = user.my_reg_crc_;
         my_username_ = user.my_username_;
         my_wizpass_ = user.my_wizpass_;
      }

      void User::do_init(void)
      {
         do_reset();
      }

      User::~User(void)
      {
         do_deinit();
      }

      User::User(QObject* pParent)
         : QObject(pParent)
      {
         do_init();
      }

      User::User(const User& user, QObject* pParent)
         : QObject(pParent)
      {
         do_init(user);
      }
   }
}
