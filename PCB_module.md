MYNRF24PCB贴片说明和细节

　　MYNRF24-CR2032板涉及元件比较多，兼容了比较多的可能性，故开贴一一说明，方便大家理解需要核贴的元器件.

##整板介绍
PCB尺寸 : 39.12mmX20.57mm 支持沿虚线切割，切割后使用尺寸20.57mmmX20.06mm

### Front pic
![mynrf24-front](https://github.com/huexpub/MYNRF24/blob/master/doc/front.png?raw=true)

### Back pic
![mynrf24-front](https://github.com/huexpub/MYNRF24/blob/master/doc/back.png?raw=true)
### package 

![package](https://github.com/huexpub/MYNRF24/raw/master/doc/package.jpg?raw=true)
　　此图为整板元件介绍，D2 D3为mcu处理单元的物理中断脚，模块支持外接供电，沿线切割使用，支持固有标准模块，比如gy-302(bh1750)、si7021迷你、支持bme280等，查看时候请使另存或者使用浏览器插件放大！PIR-AM312上元件的时候请务必不要贴板，合理避开元件，建议上高2-3mm左右

##模块选择

### 基础原板
![enter image description here](https://github.com/huexpub/MYNRF24/blob/master/doc/basic.jpg?raw=true)

####0.1uf电容3颗，10K电阻两颗，4.7uf或者47uf电容(0603/0805)一颗

![enter image description here](2222)


### 二进制传感器
![enter image description here](https://github.com/huexpub/MYNRF24/blob/master/doc/Brinary.jpg?raw=true)

####D2、D3均为支持门磁、雨滴、无线按键等，使用时请上拉1M或者2M电阻，电耗上1M>2M,

### I2C传感器
![enter image description here](https://github.com/huexpub/MYNRF24/blob/master/doc/package_area.jpg?raw=true)

####按图区域，使用时对应帖电阻电容，如果出现I2C掉压，请使用bat右边2个贴脚上拉slc和sda

### MINI模块
![enter image description here](https://github.com/huexpub/MYNRF24/blob/master/doc/MINI.jpg?raw=true)

####按板上虚线位置，使用美工刀划开，双面达到一定深度，使用工具掰开即可。





