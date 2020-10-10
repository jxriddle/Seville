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
            static const u32 kOffsetRoomPayload = \
                  kOffsetForNetMsgPayloadInBytes + 40;

            auto New(bool shouldSwapEndianness = false) -> \
                  std::optional<RoomDescription*>;

            auto Copy(const GenericNetMsg& generic_net_msg) -> \
                  std::optional<RoomDescription*>;

         private:
            auto do_reset(void) -> void;
//            {
//               truncate(kOffsetForNetMsgPayloadInBytes);
//               reserve(kSizeOfRoomDescriptionInBytes);
//               setId(netmsg::Kind::kRoomKind);
//               setLen(netmsg::kSizeOfRoomDescriptionInBytes);
//               setRef(0);
//            }

         public:
            RoomDescription(void) = delete;

            inline auto flags(void) const -> u32 {
               return u32At(kOffsetForNetMsgPayloadInBytes);
            }

            inline auto face(void) const -> u32 {
               return u32At(kOffsetForNetMsgPayloadInBytes+4);
            }

            inline auto roomId(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+8);
            }

            inline auto roomNameOffset(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+10);
            }

            inline auto backgroundImageNameOffset(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+12);
            }

            inline auto artistNameOffset(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+14);
            }

            inline auto passwordOffset(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+16);
            }

            inline auto hotspotCount(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+18);
            }

            inline auto hotspotOffset(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+20);
            }

            inline auto roomImageCount(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+22);
            }

            inline auto backgroundImageOffset(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+24);
            }

            inline auto drawCommandsCount(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+26);
            }

            inline auto firstDrawCommand(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+28);
            }

            inline auto roomUserCount(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+30);
            }

            inline auto loosePropCount(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+32);
            }

            inline auto firstLooseProp(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+34);
            }

            inline auto reserved(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+36);
            }

            inline auto roomDataLen(void) const -> u16 {
               return u16At(kOffsetForNetMsgPayloadInBytes+38);
            }

            //ByteArray roomData(void) {
            //   return byteArrayAt(kOffsetForNetMsgPayloadInBytes + 40);
            //}

//            std::vector<Palace::Prop> props(void)
//            {
//               auto value = std::vector<Palace::Prop>();

//               auto loosePropCount =  u16At(kOffsetForNetMsgPayloadInBytes+32);
//               for (auto i = 0; i < loosePropCount; i++)
//               {
//                  Prop* p = ;
//                  value.push_back(*p);
//               }

//               return value;
//            }

            auto artistName(void) const -> QString {
               return qstring_at(
                        kOffsetRoomPayload +
                        u16At(kOffsetForNetMsgPayloadInBytes+14), 1024);
            }

            auto backgroundImageName(void) const -> QString {
               return qstring_at(
                        u16At(kOffsetRoomPayload +
                              u16At(kOffsetForNetMsgPayloadInBytes+24)), 1024);
            }

            //QByteArray backgroundImage(void) {
            //   return byteArrayAt(u16At(kOffsetForNetMsgPayloadInBytes+24), 1024);
            //}
         };
      }
   }
}

#endif  // SEVILLE_PALACE_NETMSG_ROOM_H_
