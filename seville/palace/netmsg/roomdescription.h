#ifndef SEVILLE_PALACE_NETMSG_ROOM_H_
#define SEVILLE_PALACE_NETMSG_ROOM_H_

#include <vector>

#include <QByteArray>

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
               kMaximumDefaultNameSize = 1024,
               kMaximumArtistNameSize = 1024,
               kMaximumBackgroundImageNameSize = 1024,
            };

            enum RoomDescriptionOffset {
               kFlagsOffset = NetMsgOffset::kPayloadOffset,
               kFaceOffset = NetMsgOffset::kPayloadOffset+4,
               kRoomIdOffset = NetMsgOffset::kPayloadOffset+8,
               kRoomNameOffset = NetMsgOffset::kPayloadOffset+10,
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
            };

            static auto New( \
                  NetMsgOptions options = NetMsgOptions::kNoEndianSwap) \
                  -> std::unique_ptr<RoomDescription>;

            static auto Copy( \
                  const NetMsg& netmsg) \
                  -> std::unique_ptr<RoomDescription>;

            RoomDescription(void);

            inline auto flags(void) const -> u32 {
               return rep_ref_().u32_at(kFlagsOffset);
            }

            inline auto face(void) const -> u32 {
               return rep_ref_().u32_at(kFaceOffset);
            }

            inline auto room_id(void) const -> u16 {
               return rep_ref_().u16_at(kRoomIdOffset);
            }

            inline auto room_name_offset(void) const -> u16 {
               return rep_ref_().u16_at(kRoomNameOffset);
            }

            inline auto background_image_name_offset(void) const -> u16 {
               return rep_ref_().u16_at(kBackgroundImageNameOffsetOffset);
            }

            inline auto artist_name_offset(void) const -> u16 {
               return rep_ref_().u16_at(kArtistNameOffsetOffset);
            }

            inline auto password_offset(void) const -> u16 {
               return rep_ref_().u16_at(kPasswordOffsetOffset);
            }

            inline auto hotspot_count(void) const -> u16 {
               return rep_ref_().u16_at(kHotspotCountOffset);
            }

            inline auto hotspot_offset(void) const -> u16 {
               return rep_ref_().u16_at(kHotspotOffsetOffset);
            }

            inline auto room_image_count(void) const -> u16 {
               return rep_ref_().u16_at(kRoomImageCountOffset);
            }

            inline auto background_image_offset(void) const -> u16 {
               return rep_ref_().u16_at(kBackgroundImageOffsetOffset);
            }

            inline auto draw_commands_count(void) const -> u16 {
               return rep_ref_().u16_at(kDrawCommandsCountOffset);
            }

            inline auto first_draw_command(void) const -> u16 {
               return rep_ref_().u16_at(kFirstDrawCommandOffset);
            }

            inline auto room_user_count(void) const -> u16 {
               return rep_ref_().u16_at(kRoomUserCountOffset);
            }

            inline auto loose_prop_count(void) const -> u16 {
               return rep_ref_().u16_at(kLoosePropCountOffset);
            }

            inline auto first_loose_prop(void) const -> u16 {
               return rep_ref_().u16_at(kFirstLoosePropOffset);
            }

            inline auto reserved(void) const -> u16 {
               return rep_ref_().u16_at(kReservedOffset);
            }

            inline auto room_data_len(void) const -> u16 {
               return rep_ref_().u16_at(kRoomDataLenOffset);
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
                     rep_ref_().qstring_at( \
                        kPayloadOffset + do_artist_name_offset_(), \
                        kMaximumArtistNameSize);

               return result;
            }

            auto background_image_name(void) const -> QString {
                auto result = \
                      rep_ref_().qstring_at( \
                         kPayloadOffset + do_background_image_name_offset_(), \
                         kMaximumBackgroundImageNameSize);

                return result;
            }

            //QByteArray backgroundImage(void) {
            //   return byteArrayAt(u16_at(NetMsgOffset::kPayload+24), 1024);
            //}

         private:
            auto do_reset_(void) -> void;

            auto do_set_options_(NetMsgOptions options);

            inline auto do_flags_(void) const -> u32 {
               return rep_ref_().u32_at(kFlagsOffset);
            }

            inline auto do_face_(void) const -> u32 {
               return rep_ref_().u32_at(kFaceOffset);
            }

            inline auto do_room_id_(void) const -> u16 {
               return rep_ref_().u16_at(kRoomIdOffset);
            }

            inline auto do_room_name_offset_(void) const -> u16 {
               return rep_ref_().u16_at(kRoomNameOffset);
            }

            inline auto do_background_image_name_offset_(void) const -> u16 {
               return rep_ref_().u16_at(kBackgroundImageNameOffsetOffset);
            }

            inline auto do_artist_name_offset_(void) const -> u16 {
               return rep_ref_().u16_at(kArtistNameOffsetOffset);
            }

            inline auto do_password_offset_(void) const -> u16 {
               return rep_ref_().u16_at(kPasswordOffsetOffset);
            }

            inline auto do_hotspot_count_(void) const -> u16 {
               return rep_ref_().u16_at(kHotspotCountOffset);
            }

            inline auto do_hotspot_offset_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+20);
            }

            inline auto do_room_image_count_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+22);
            }

            inline auto do_background_image_offset_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+24);
            }

            inline auto do_draw_commands_count_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+26);
            }

            inline auto do_first_draw_command_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+28);
            }

            inline auto do_room_user_count_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+30);
            }

            inline auto do_loose_prop_count_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+32);
            }

            inline auto do_first_loose_prop_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+34);
            }

            inline auto do_reserved_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+36);
            }

            inline auto do_room_data_len_(void) const -> u16 {
               return rep_ref_().u16_at(NetMsgOffset::kPayloadOffset+38);
            }
         };
      }
   }
}

#endif  // SEVILLE_PALACE_NETMSG_ROOM_H_
