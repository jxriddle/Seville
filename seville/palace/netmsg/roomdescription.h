#ifndef SEVILLE_PALACE_NETMSG_ROOM_H_
#define SEVILLE_PALACE_NETMSG_ROOM_H_

#include <vector>

#include <QByteArray>
#include <QImage>

//#include "seville/base/app.h"

#include "seville/palace/base/prop.h"
#include "seville/palace/base/netmsg.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
         class RoomDescription : public NetMsg
         {
         public:
            enum RoomDescriptionSize {
               kMaximumDefaultNameSize = 255,
               kMaximumArtistNameSize = 255,
               kMaximumBackgroundImageNameSize = 255,
            };

            enum RoomDescriptionOffset {
               kFlagsOffset = NetMsgOffset::kPayloadOffset,
               kFaceOffset = NetMsgOffset::kPayloadOffset+4,
               kRoomIdOffset = NetMsgOffset::kPayloadOffset+8,
               kRoomNameOffsetOffset = NetMsgOffset::kPayloadOffset+10,
               kBackgroundImageNameOffsetOffset =
                     NetMsgOffset::kPayloadOffset+12,
               kArtistNameOffsetOffset = NetMsgOffset::kPayloadOffset+14,
               kPasswordOffsetOffset = NetMsgOffset::kPayloadOffset+16,
               kHotspotCountOffset = NetMsgOffset::kPayloadOffset+18,
               kHotspotOffsetOffset = NetMsgOffset::kPayloadOffset+20,
               kRoomImageCountOffset = NetMsgOffset::kPayloadOffset+22,
               kBackgroundImageOffsetOffset = NetMsgOffset::kPayloadOffset+24,
               kDrawCommandsCountOffset = NetMsgOffset::kPayloadOffset+26,
               kFirstDrawCommandOffset = NetMsgOffset::kPayloadOffset+28,
               kRoomUserCountOffset = NetMsgOffset::kPayloadOffset+30,
               kLoosePropCountOffset = NetMsgOffset::kPayloadOffset+32,
               kFirstLoosePropOffset = NetMsgOffset::kPayloadOffset+34,
               kReservedOffset = NetMsgOffset::kPayloadOffset+36,
               kRoomDataLenOffset = NetMsgOffset::kPayloadOffset+38,
               kRoomData = NetMsgOffset::kPayloadOffset+40,
            };

            static auto New(
                  NetMsgOptions options = NetMsgOptions::kNoEndianSwap)
                  -> RoomDescription*;

            static auto Copy(
                  const NetMsg& netmsg)
                  -> std::unique_ptr<RoomDescription>;

            RoomDescription(void);

            RoomDescription(
                  NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

            inline auto flags(void) const -> u32 {
               return do_flags_();
            }

            inline auto face(void) const -> u32 {
               return do_face_();
            }

            inline auto room_id(void) const -> u16 {
               return do_room_id_();
            }

            inline auto room_name_offset(void) const -> u16 {
               return do_room_name_offset_();
            }

            inline auto background_image_name_offset(void) const -> u16 {
               return do_background_image_name_offset_();
            }

            inline auto artist_name_offset(void) const -> u16 {
               return do_artist_name_offset_();
            }

            inline auto password_offset(void) const -> u16 {
               return do_password_offset_();
            }

            inline auto hotspot_count(void) const -> u16 {
               return do_hotspot_count_();
            }

            inline auto hotspot_offset(void) const -> u16 {
               return do_hotspot_offset_();
            }

            inline auto room_image_count(void) const -> u16 {
               return do_room_image_count_();
            }

            inline auto background_image_offset(void) const -> u16 {
               return do_background_image_offset_();
            }

            inline auto draw_commands_count(void) const -> u16 {
               return do_draw_commands_count_();
            }

            inline auto first_draw_command(void) const -> u16 {
               return do_first_draw_command_();
            }

            inline auto room_user_count(void) const -> u16 {
               return do_room_user_count_();
            }

            inline auto loose_prop_count(void) const -> u16 {
               return do_loose_prop_count_();
            }

            inline auto first_loose_prop(void) const -> u16 {
               return do_first_loose_prop_();
            }

            inline auto reserved(void) const -> u16 {
               return do_reserved_();
            }

            inline auto room_data_len(void) const -> u16 {
               return do_room_data_len_();
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

            inline auto artist_name(void) const -> QString {
               return qstring_at(
                        kRoomData + do_artist_name_offset_(),
                        kMaximumArtistNameSize);
            }

            inline auto background_image_name(void) const -> QString {
               return pascal_qstring_at(
                        kRoomData + do_background_image_name_offset_());
                        //kMaximumBackgroundImageNameSize);
            }

//            auto background_image(void) const -> QImage {
//               auto image_byte_array = rep_ref_().mid(
//                        kPayloadOffset + do_background_image_offset_(),
//                        );

//               QImage background_q_image(image_byte_array);

//               return background_q_image;
//            }

            //QByteArray backgroundImage(void) {
            //   return byteArrayAt(u16_at(NetMsgOffset::kPayload+24), 1024);
            //}

         private:
            auto do_reset_(void) -> void;

            auto do_set_options_(NetMsgOptions options);

            inline auto do_flags_(void) const -> u32 {
               return u32_at(kFlagsOffset);
            }

            inline auto do_face_(void) const -> u32 {
               return u32_at(kFaceOffset);
            }

            inline auto do_room_id_(void) const -> u16 {
               return u16_at(kRoomIdOffset);
            }

            inline auto do_room_name_offset_(void) const -> u16 {
               return u16_at(kRoomNameOffsetOffset);
            }

            inline auto do_background_image_name_offset_(void) const -> u16 {
               return u16_at(kBackgroundImageNameOffsetOffset);
            }

            inline auto do_artist_name_offset_(void) const -> u16 {
               return u16_at(kArtistNameOffsetOffset);
            }

            inline auto do_password_offset_(void) const -> u16 {
               return u16_at(kPasswordOffsetOffset);
            }

            inline auto do_hotspot_count_(void) const -> u16 {
               return u16_at(kHotspotCountOffset);
            }

            inline auto do_hotspot_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+20);
            }

            inline auto do_room_image_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+22);
            }

            inline auto do_background_image_offset_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+24);
            }

            inline auto do_draw_commands_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+26);
            }

            inline auto do_first_draw_command_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+28);
            }

            inline auto do_room_user_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+30);
            }

            inline auto do_loose_prop_count_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+32);
            }

            inline auto do_first_loose_prop_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+34);
            }

            inline auto do_reserved_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+36);
            }

            inline auto do_room_data_len_(void) const -> u16 {
               return u16_at(NetMsgOffset::kPayloadOffset+38);
            }
         };
      }
   }
}

#endif  // SEVILLE_PALACE_NETMSG_ROOM_H_
