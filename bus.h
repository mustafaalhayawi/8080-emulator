#include <functional>

class IDevice 
{
public:
    virtual uint8_t read() = 0;
    virtual void write(uint8_t val) = 0;
};

class EmptyDevice : public IDevice
{
public:
    virtual uint8_t read() override { return 0xFF; }
    virtual void write(uint8_t val) override { }
};

class Bus
{
private:
    EmptyDevice m_empty;
public:
    IDevice* ports[256];
    void attach_device(uint8_t port, IDevice* device) { ports[port] = device; }

    Bus() {
        for (int i=0; i<256; i++) {
            ports[i] = &m_empty;
        }
    }
};
