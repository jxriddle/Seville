#ifndef SEVILLE_PALACE_USER_H_
#define SEVILLE_PALACE_USER_H_

#include <QImage>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "seville/base/types.h"
#include "seville/palace/prop.h"

namespace seville
{
   namespace palace
   {
      class User : QObject
      {
      Q_OBJECT

      public:
         static const i32 kNumPropCells = 9;

         User(QObject* parentPtr = nullptr);
         User(const User& user, QObject* parentObjectPtr = nullptr);

         virtual ~User(void);

         User& operator=(const User& value);

         inline auto color(void) const -> int {
            return my_color;
         }

         inline auto setColor(int value) -> void {
            my_color = value;
         }

         inline auto face(void) const -> int {
            return my_face;
         }

         inline auto setFace(int value) -> void {
            my_face = value;
         }

         inline auto x(void) const -> int {
            return my_x;
         }

         inline auto setX(int value) -> void {
            my_x = value;
         }

         inline auto y(void) const -> int {
            return my_y;
         }

         inline auto setY(int value) -> void {
            my_y = value;
         }

         inline auto username(void) const -> QString {
            return my_username;
         }

         inline auto setUsername(const QString& value) -> void {
            my_username = value;
         }

         inline auto wizPass(void) const -> QString {
            return my_wizardPassword;
         }

         inline auto setWizPass(const QString& value) -> void {
            my_wizardPassword = value;
         }

         inline auto id(void) const -> u32 {
            return my_id;
         }

         inline auto setId(u32 value) -> void {
            my_id = value;
         }

         inline auto flags(void) -> u16 {
            return my_flags;
         }

         inline auto setFlags(u16 value) -> void {
            my_flags = value;
         }

         inline auto idCrc(void) const -> u32 {
            return my_idCrc;
         }

         inline auto setIdCrc(u32 value) -> void {
            my_idCrc = value;
         }

         inline auto idCounter(void) const -> u32 {
            return my_idCounter;
         }

         inline auto setIdCounter(u32 value) -> void {
            my_idCounter = value;
         }

         inline auto idChanged(void) const -> bool {
            return my_idChangedFlag;
         }

         inline auto setIdChanged(bool value) -> void {
            my_idChangedFlag = value;
         }

         inline auto regCrc(void) const -> u32 {
            return my_regCrc;
         }

         inline auto setRegCrc(u32 value) -> void {
            my_regCrc = value;
         }

         inline auto regCounter(void) const -> u32 {
            return my_regCounter;
         }

         inline auto setRegCounter(u32 value) -> void {
            my_regCounter = value;
         }

         inline auto roomId(void) const -> i16 {
            return my_roomId;
         }

         inline auto setRoomId(i16 id) -> void {
            my_roomId = id;
         }

         inline auto propListPtr(void) -> std::vector<Prop>* {
            return &my_propList;
         }

         inline auto propNum(void) -> i16 {
            return my_propNum;
         }

         inline auto setPropNum(i16 value) {
            my_propNum = value;
         }

         inline auto clear(void) -> void {
            do_clear();
         }

         inline auto hasHeadProp(void) -> u32 {
            return do_hasHeadProp();
         }

//         inline auto propsPtr(void) -> u32* {
//            return &my_props;
//         }

      public slots:
         void on_webPropDidFetchAsync(QNetworkReply* replyPtr);

      private:
         u32 my_id;
         u16 my_flags;
         QString my_username;
         QString my_wizardPassword;
         int my_face;
         int my_x;
         int my_y;
         int my_color;
         bool my_idChangedFlag;
         u32 my_idCounter;
         u32 my_idCrc;
         u32 my_regCounter;
         u32 my_regCrc;
         i16 my_roomId;
         std::vector<Prop> my_propList;
         i16 my_propNum;
         u32 my_headPropFlag;
         QImage my_webPropImage;
         QNetworkAccessManager my_webPropFetcher;

         //u32 my_props[kNumPropCells];

         auto do_hasHeadProp(void) -> u32;
         auto do_fetchWebPropAsync(void) -> void;

         auto do_clear(void) -> void;
         auto do_assign(const User& user) -> void;

         auto do_deinit(void) -> void;
         auto do_init(void) -> void;

         auto do_init(const User& user) -> void {
            do_assign(user);
         }
      };
   }
}

#endif   // SEVILLE_PALACE_USER_H
