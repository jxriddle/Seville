#include "user.h"

namespace seville
{
   namespace palace
   {
      void User::do_reset(void)
      {
         my_color = 1;
         my_face = 1;
         my_x = 0;
         my_y = 0;
         my_idChanged = false;
         my_idCounter = 0xf5dc385e;
         my_idCrc = 0xc144c580;
         my_regCounter = 0xcf07309c;
         my_regCrc = 0x5905f923;
         my_username = "Seville User";
         my_wizpass = "";
      }

      void User::do_assign(const User& user)
      {
         my_color = user.my_color;
         my_face = user.my_face;
         my_x = user.my_x;
         my_y = user.my_y;
         my_idChanged = user.my_idChanged;
         my_idCounter = user.my_idCounter;
         my_idCrc = user.my_idCrc;
         my_regCounter = user.my_regCounter;
         my_regCrc = user.my_regCrc;
         my_username = user.my_username;
         my_wizpass = user.my_wizpass;
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
