#include <memory>

#include "seville/palace/netmsg.h"
#include "seville/palace_netmsg/roomdescription.h"

namespace seville
{
   namespace palace
   {
      namespace netmsg
      {
//         RoomDescriptionObject::RoomDescriptionObject(bool shouldSwapEndianness)
//            : GenericNetMsg(shouldSwapEndianness)
//         {
//            do_reset();
//         }

//         RoomDescriptionObject::RoomDescriptionObject(const GenericNetMsg& netMsg, bool shouldSwapEndianness)
//            : GenericNetMsg(netMsg, shouldSwapEndianness)
//         {
//            do_reset();
//         }

         auto RoomDescriptionNetMsg::New(NetMsgOptions options) ->
            RoomDescriptionNetMsg*
         {
            auto instance = new RoomDescriptionNetMsg(options);
            //instance.set_options(options);
//            if (instance.has_value())
//               instance.value()->
//                     setShouldSwapEndiannessFlag(shouldSwapEndianness);

            return instance;
         }

         RoomDescriptionNetMsg::RoomDescriptionNetMsg(void)
         {
            do_reset();
         }

         RoomDescriptionNetMsg::RoomDescriptionNetMsg(NetMsgOptions options)
         {
            do_reset();
            SetShouldSwapEndianness(options & NetMsgOptions::kEndianSwap);
         }

//         auto RoomDescriptionObject::Copy(const NetMsg& netmsg)
//               -> std::optional<RoomDescriptionObject*>
//         {
//            auto instance = std::make_optional<RoomDescriptionObject*>();
//            if (instance.has_value())
//               instance.value()->setShouldSwapEndiannessFlag(
//                        generic_net_msg.shouldSwapEndiannessFlag());

//            return instance;
//         }

         auto RoomDescriptionNetMsg::do_reset(void) -> void
         {
            truncate(NetMsgSize::kMinimumSize);
            reserve(NetMsgSize::kRoomDescriptionNetMsgSize);

            SetId(NetMsgKind::kRoomKind);
            SetLen(NetMsgSize::kRoomDescriptionNetMsgSize);
            SetRef(0);
         }
      }
   }
}
