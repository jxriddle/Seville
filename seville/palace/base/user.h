#ifndef SEVILLE_PALACE_USER_H_
#define SEVILLE_PALACE_USER_H_

#include <QObject>

#include "seville/base/types.h"

namespace seville
{
   namespace palace
   {
      class User : QObject
      {
      Q_OBJECT

      public:
         User(QObject* object_parent_ptr = nullptr);
         User(const User& user, QObject* object_parent_ptr = nullptr);

         virtual ~User(void);

         inline auto color(void) const -> int {
            return my_color_;
         }

         inline auto setColor(int value) -> void {
            my_color_ = value;
         }

         inline auto face(void) const -> int {
            return my_face_;
         }

         inline auto setFace(int value) -> void {
            my_face_ = value;
         }

         inline auto x(void) const -> int {
            return my_x_;
         }

         inline auto setX(int value) -> void {
            my_x_ = value;
         }

         inline auto y(void) const -> int {
            return my_y_;
         }

         inline auto setY(int value) -> void {
            my_y_ = value;
         }

         inline auto username(void) const -> QString {
            return my_username_;
         }

         inline auto setUsername(const QString& value) -> void {
            my_username_ = value;
         }

         inline auto wizpass(void) const -> QString {
            return my_wizpass_;
         }

         inline auto setWizpass(const QString& value) -> void {
            my_wizpass_ = value;
         }

         inline auto id(void) const -> u32 {
            return my_id_;
         }

         inline auto setId(u32 value) -> void {
            my_id_ = value;
         }

         inline auto idCrc(void) const -> u32 {
            return my_id_crc_;
         }

         inline auto setIdCrc(u32 value) -> void {
            my_id_crc_ = value;
         }

         inline auto idCounter(void) const -> u32 {
            return my_id_counter_;
         }

         inline auto setIdCounter(u32 value) -> void {
            my_id_counter_ = value;
         }

         inline auto idChanged(void) const -> bool {
            return my_id_changed_;
         }

         inline auto setIdChanged(bool value) -> void {
            my_id_changed_ = value;
         }

         inline auto regCrc(void) const -> u32 {
            return my_reg_crc_;
         }

         inline auto setRegCrc(u32 value) -> void {
            my_reg_crc_ = value;
         }

         inline auto regCounter(void) const -> u32 {
            return my_reg_counter_;
         }

         inline auto setRegCounter(u32 value) -> void {
            my_reg_counter_ = value;
         }

         inline auto reset(void) -> void {
            do_reset();
         }

      private:
         u32 my_id_;
         QString my_username_;
         QString my_wizpass_;
         int my_face_;
         int my_x_;
         int my_y_;
         int my_color_;
         bool my_id_changed_;
         u32 my_id_counter_;
         u32 my_id_crc_;
         u32 my_reg_counter_;
         u32 my_reg_crc_;

         auto do_reset(void) -> void;
         auto do_assign(const User& user) -> void;

         inline auto do_deinit(void) -> void {

         }

         inline auto do_init(void) -> void;

         inline auto do_init(const User& user) -> void {
            do_assign(user);
         }
      };
   }
}

#endif   // SEVILLE_PALACE_USER_H
