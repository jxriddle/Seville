#ifndef PALACE_USER_H
#define PALACE_USER_H

#include <QObject>

#include "zog/base/types.h"

namespace seville
{
   namespace palace
   {
      class User : QObject
      {
      Q_OBJECT

      private:
         zog::u32 my_id;
         QString my_username;
         QString my_wizpass;
         int my_face;
         int my_x;
         int my_y;
         int my_color;
         bool my_idChanged;
         zog::u32 my_idCounter;
         zog::u32 my_idCrc;
         zog::u32 my_regCounter;
         zog::u32 my_regCrc;

         void do_reset(void);
         void do_assign(const User& user);

         void do_deinit(void)
         {

         }

         void do_init(void);

         void do_init(const User& user)
         {
            do_assign(user);
         }

      public:
         void setColor(int value) { my_color = value; }
         int color(void) const { return my_color; }

         void setFace(int value) { my_face = value; }
         int face(void) const { return my_face; }

         void setX(int value) { my_x = value; }
         int x(void) const { return my_x; }

         void setY(int value) { my_y = value; }
         int y(void) const { return my_y; }

         void setUsername(const QString& value) { my_username = value; }
         QString username(void) const { return my_username; }

         void setWizpass(const QString& value) { my_wizpass = value; }
         QString wizpass(void) const { return my_wizpass; }

         void setId(zog::u32 value) { my_id = value; }
         zog::u32 id(void) const { return my_id; }

         void setIdCrc(zog::u32 value) { my_idCrc = value; }
         zog::u32 idCrc(void) const { return my_idCrc; }

         void setIdCounter(zog::u32 value) { my_idCounter = value; }
         zog::u32 idCounter(void) const { return my_idCounter; }

         void setIdChanged(bool value) { my_idChanged = value; }
         bool idChanged(void) const { return my_idChanged; }

         void setRegCrc(zog::u32 value) { my_regCrc = value; }
         zog::u32 regCrc(void) const { return my_regCrc; }

         void setRegCounter(zog::u32 value) { my_regCounter = value; }
         zog::u32 regCounter(void) const { return my_regCounter; }

         void reset(void) { do_reset(); }

         virtual ~User(void);
         User(QObject* pParent = nullptr);
         User(const User& user, QObject* pParent = nullptr);
      };
   }
}

#endif // PALACE_USER_H
