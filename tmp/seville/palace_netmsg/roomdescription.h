#ifndef SEVILLE_PALACE_NETMSG_ROOM_H_
#define SEVILLE_PALACE_NETMSG_ROOM_H_

#include <vector>

#include <QByteArray>
#include <QImage>

//#include "seville/base/app.h"

#include "seville/palace/prop.h"
#include "seville/palace/netmsg.h"

namespace seville
{
   namespace palace_netmsg
   {
      class RoomDescriptionNetMsg : public palace::NetMsg
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
               -> RoomDescriptionNetMsg*;

         static auto Copy(
               const NetMsg& netmsg)
               -> std::unique_ptr<RoomDescriptionNetMsg>;

         RoomDescriptionNetMsg(void);

         RoomDescriptionNetMsg(
               NetMsgOptions options = NetMsgOptions::kNoEndianSwap);

         inline auto Flags(void) const -> u32 {
            return do_flags();
         }

         inline auto Face(void) const -> u32 {
            return do_face();
         }

         inline auto RoomId(void) const -> u16 {
            return do_room_id();
         }

         inline auto RoomNameOffset(void) const -> u16 {
            return do_room_name_offset();
         }

         inline auto BackgroundImageNameOffset(void) const -> u16 {
            return do_background_image_name_offset();
         }

         inline auto ArtistNameOffset(void) const -> u16 {
            return do_artist_name_offset();
         }

         inline auto PasswordOffset(void) const -> u16 {
            return do_password_offset();
         }

         inline auto HotspotCount(void) const -> u16 {
            return do_hotspot_count();
         }

         inline auto HotspotOffset(void) const -> u16 {
            return do_hotspot_offset();
         }

         inline auto RoomImageCount(void) const -> u16 {
            return do_room_image_count();
         }

         inline auto BackgroundImageOffset(void) const -> u16 {
            return do_background_image_offset();
         }

         inline auto DrawCommandsCount(void) const -> u16 {
            return do_draw_commands_count();
         }

         inline auto FirstDrawCommand(void) const -> u16 {
            return do_first_draw_command();
         }

         inline auto RoomUserCount(void) const -> u16 {
            return do_room_user_count();
         }

         inline auto LoosePropCount(void) const -> u16 {
            return do_loose_prop_count();
         }

         inline auto FirstLooseProp(void) const -> u16 {
            return do_first_loose_prop();
         }

         inline auto Reserved(void) const -> u16 {
            return do_reserved();
         }

         inline auto RoomDataLen(void) const -> u16 {
            return do_room_data_len();
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

         inline auto ArtistName(void) const -> QString {
            return qstringAt(
                     kRoomData + do_artist_name_offset(),
                     kMaximumArtistNameSize);
         }

         inline auto BackgroundImageName(void) const -> QString {
            return pascalQStringAt(
                     kRoomData + do_background_image_name_offset());
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
         auto do_reset(void) -> void;

         auto do_set_options(NetMsgOptions options);

         inline auto do_flags(void) const -> u32 {
            return u32At(kFlagsOffset);
         }

         inline auto do_face(void) const -> u32 {
            return u32At(kFaceOffset);
         }

         inline auto do_room_id(void) const -> u16 {
            return u16At(kRoomIdOffset);
         }

         inline auto do_room_name_offset(void) const -> u16 {
            return u16At(kRoomNameOffsetOffset);
         }

         inline auto do_background_image_name_offset(void) const -> u16 {
            return u16At(kBackgroundImageNameOffsetOffset);
         }

         inline auto do_artist_name_offset(void) const -> u16 {
            return u16At(kArtistNameOffsetOffset);
         }

         inline auto do_password_offset(void) const -> u16 {
            return u16At(kPasswordOffsetOffset);
         }

         inline auto do_hotspot_count(void) const -> u16 {
            return u16At(kHotspotCountOffset);
         }

         inline auto do_hotspot_offset(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+20);
         }

         inline auto do_room_image_count(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+22);
         }

         inline auto do_background_image_offset(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+24);
         }

         inline auto do_draw_commands_count(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+26);
         }

         inline auto do_first_draw_command(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+28);
         }

         inline auto do_room_user_count(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+30);
         }

         inline auto do_loose_prop_count(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+32);
         }

         inline auto do_first_loose_prop(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+34);
         }

         inline auto do_reserved(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+36);
         }

         inline auto do_room_data_len(void) const -> u16 {
            return u16At(NetMsgOffset::kPayloadOffset+38);
         }
      };
   }
}

#endif  // SEVILLE_PALACE_NETMSG_ROOM_H_
