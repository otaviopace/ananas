/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2009-2018 Rink Springer <rink@rink.nu>
 * For conditions of distribution and use, see LICENSE file
 */
#ifndef ANANAS_VCONSOLE_H
#define ANANAS_VCONSOLE_H

#include <ananas/types.h>
#include <ananas/util/array.h>
#include "kernel/device.h"
#include "kernel/dev/kbdmux.h"

class IVideo;
class VTTY;

struct VConsole : public Device,
                  private IDeviceOperations,
                  private ICharDeviceOperations,
                  private keyboard_mux::IKeyboardConsumer {
    using Device::Device;
    virtual ~VConsole() = default;

    IDeviceOperations& GetDeviceOperations() override { return *this; }

    ICharDeviceOperations* GetCharDeviceOperations() override { return this; }

    IVideo& GetVideo() { return *v_Video; }

    Result Attach() override;
    Result Detach() override;

    Result Read(void* buf, size_t len, off_t offset) override;
    Result Write(const void* buf, size_t len, off_t offset) override;

    void OnKey(const keyboard_mux::Key& key, int modifier) override;

  private:
    constexpr static size_t NumberOfVTTYs = 4;
    util::array<VTTY*, NumberOfVTTYs> vttys;

    IVideo* v_Video = nullptr;
    VTTY* activeVTTY = nullptr;
};

#endif /* ANANAS_VCONSOLE_H */
