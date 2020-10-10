#include "seville/palace/base/genericnetmsg.h"
#include "seville/palace/netmsg/roomdescription.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
//         RoomDescription::RoomDescription(bool shouldSwapEndianness)
//            : GenericNetMsg(shouldSwapEndianness)
//         {
//            do_reset();
//         }

//         RoomDescription::RoomDescription(const GenericNetMsg& netMsg, bool shouldSwapEndianness)
//            : GenericNetMsg(netMsg, shouldSwapEndianness)
//         {
//            do_reset();
//         }

         auto RoomDescription::newInstance(bool shouldSwapEndianness)
               -> std::optional<RoomDescription*>
         {
            auto instance = std::make_optional<RoomDescription*>();
            if (instance.has_value())
               instance.value()->
                     setShouldSwapEndiannessFlag(shouldSwapEndianness);

            return instance;
         }

         auto RoomDescription::cloneInstance(
               const GenericNetMsg& generic_net_msg)
               -> std::optional<RoomDescription*>
         {
            auto instance = std::make_optional<RoomDescription*>();
            if (instance.has_value())
               instance.value()->setShouldSwapEndiannessFlag(
                        generic_net_msg.shouldSwapEndiannessFlag());

            return instance;
         }

         auto RoomDescription::do_reset(void) -> void
         {
            truncate(kOffsetForPayloadInBytes);
            reserve(kSizeOfRoomDescriptionInBytes);
            setId(netmsg::NetMsgKind::kRoomKind);
            setLen(netmsg::kSizeOfRoomDescriptionInBytes);
            setRef(0);
         }
      }
   }
}
