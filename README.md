# Secure OS

Much sekure.

## Usage

### Docker

```bash
mkdir build
docker-compose build && docker-compose up # build the thing
sudo chmod +wrx build # permissions ig
sudo qemu-system-i386 -drive format=raw,file=build/os_image # run the thing
```
