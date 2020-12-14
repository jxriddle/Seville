#include <memory>

#include "seville/palace/base/netmsg.h"
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

         auto RoomDescription::New(NetMsgOptions options) -> RoomDescription*
         {
            auto instance = new RoomDescription(options);
            //instance.set_options(options);
//            if (instance.has_value())
//               instance.value()->
//                     setShouldSwapEndiannessFlag(shouldSwapEndianness);

            return instance;
         }

         RoomDescription::RoomDescription(void)
         {
            do_reset_();
         }

         RoomDescription::RoomDescription(NetMsgOptions options)
         {
            do_reset_();

            set_should_swap_endianness_flag(
                     options & NetMsgOptions::kEndianSwap);
         }

//         auto RoomDescription::Copy(const NetMsg& netmsg)
//               -> std::optional<RoomDescription*>
//         {
//            auto instance = std::make_optional<RoomDescription*>();
//            if (instance.has_value())
//               instance.value()->setShouldSwapEndiannessFlag(
//                        generic_net_msg.shouldSwapEndiannessFlag());

//            return instance;
//         }

         auto RoomDescription::do_reset_(void) -> void
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kRoomDescriptionSize);

            set_id(NetMsgKind::kRoomKind);
            set_len(NetMsgSize::kRoomDescriptionSize);
            set_ref(0);
         }
      }
   }
}
