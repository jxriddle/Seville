#ifndef SEVILLE_PALACE_NETMSG_ROOM_H_
#define SEVILLE_PALACE_NETMSG_ROOM_H_

#include <vector>

#include <QByteArray>

#include "seville/palace/base/prop.h"
#include "seville/palace/base/genericnetmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class RoomDescription : public GenericNetMsg
         {
         public:
            static auto New( \
                  NetMsgOptions options = NetMsgOptions::kDoNotSwapEndian) \
                  -> std::unique_ptr<RoomDescription>;

            static auto Copy( \
                  const GenericNetMsg& generic_netmsg) \
                  -> std::unique_ptr<RoomDescription>;

            RoomDescription(void);

            inline auto flags(void) const -> u32 {
               return u32_at(NetMsgOffset::kPayload);
            }

            inline auto face(void) const -> u32 {
               return u32_at(NetMsgOffset::kPayload+4);
            }

            inline auto room_id(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+8);
            }

            inline auto room_name_offset(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+10);
            }

            inline auto background_image_name_offset(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+12);
            }

            inline auto artist_name_offset(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+14);
            }

            inline auto password_offset(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+16);
            }

            inline auto hotspot_count(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+18);
            }

            inline auto hotspot_offset(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+20);
            }

            inline auto room_image_count(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+22);
            }

            inline auto background_image_offset(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+24);
            }

            inline auto draw_commands_count(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+26);
            }

            inline auto first_draw_command(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+28);
            }

            inline auto room_user_count(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+30);
            }

            inline auto loose_prop_count(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+32);
            }

            inline auto first_loose_prop(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+34);
            }

            inline auto reserved(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+36);
            }

            inline auto room_data_len(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+38);
            }

            //ByteArray roomData(void) {
            //   return byteArrayAt(NetMsgOffset::kPayload + 40);
            //}

//            std::vector<Palace::Prop> props(void)
//            {
//               auto value = std::vector<Palace::Prop>();

//               auto loosePropCount =  u16_at(NetMsgOffset::kPayload+32);
//               for (auto i = 0; i < loosePropCount; i++)
//               {
//                  Prop* p = ;
//                  value.push_back(*p);
//               }

//               return value;
//            }

            auto artist_name(void) const -> QString {
               auto result = \
                     qstring_at( \
                        NetMsgOffset::kPayload + \
                        do_artist_name_offset_(), 1024);

               return result;
            }

            auto background_image_name(void) const -> QString {
                auto result = \
                      qstring_at( \
                        NetMsgOffset::kPayload +
                              do_background_image_name_offset_(), 1024);

                return result;
            }

            //QByteArray backgroundImage(void) {
            //   return byteArrayAt(u16_at(NetMsgOffset::kPayload+24), 1024);
            //}

         private:
            auto do_reset_(void) -> void;
//            {
//               truncate(NetMsgOffset::kPayload);
//               reserve(kSizeOfRoomDescriptionInBytes);
//               setId(netmsg::Kind::kRoomKind);
//               setLen(netmsg::kSizeOfRoomDescriptionInBytes);
//               setRef(0);
//            }

            inline auto do_flags_(void) const -> u32 {
               return u32_at(NetMsgOffset::kPayload);
            }

            inline auto do_face_(void) const -> u32 {
               return u32_at(NetMsgOffset::kPayload+4);
            }

            inline auto do_room_id_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+8);
            }

            inline auto do_room_name_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+10);
            }

            inline auto do_background_image_name_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+12);
            }

            inline auto do_artist_name_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+14);
            }

            inline auto do_password_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+16);
            }

            inline auto do_hotspot_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+18);
            }

            inline auto do_hotspot_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+20);
            }

            inline auto do_room_image_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+22);
            }

            inline auto do_background_image_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+24);
            }

            inline auto do_draw_commands_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+26);
            }

            inline auto do_first_draw_command_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+28);
            }

            inline auto do_room_user_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+30);
            }

            inline auto do_loose_prop_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+32);
            }

            inline auto do_first_loose_prop_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+34);
            }

            inline auto do_reserved_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+36);
            }

            inline auto do_room_data_len_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayload+38);
            }
         };
      }
   }
}

#endif  // SEVILLE_PALACE_NETMSG_ROOM_H_
