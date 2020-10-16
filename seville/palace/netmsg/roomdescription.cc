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

         auto RoomDescription::New(NetMsgOptions options)
               -> std::unique_ptr<RoomDescription>
         {
            auto instance = std::make_unique<RoomDescription>();
            instance.set_options(options);
//            if (instance.has_value())
//               instance.value()->
//                     setShouldSwapEndiannessFlag(shouldSwapEndianness);

            return instance;
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
            rep_ref_mut_().truncate(NetMsgSize::kMinimumSize);
            rep_ref_mut_().reserve(kRoomDescriptionSize);
            rep_ref_mut_().set_id(netmsg::NetMsgKind::kRoomKind);
            rep_ref_mut_().set_len(netmsg::kSizeOfRoomDescriptionInBytes);
            rep_ref_mut_().set_ref(0);
         }
      }
   }
}
