MY APE
======

INDEX
-----
[TOC]


### 版本

#### Release Note

##### V1.0.1

原计划conscious/unconscious共享同一个neuronal net的数据，这会引起两个线程的数据竞争。所会有个读写锁的做成，但C++ 11读写锁的支持没有，所以这个事儿也没做完。

###### Feature List

- [x] Ape/brain/cortex/memory，初步实现。
- [x] Ape/brann/Cortex/instinct/mind初步实现。
- [x] Axon的building移到了minding层。
- [x] AI/TSMsgQueue已隔离出来。
- [x] ape工程编译时缺少头文件，已补全，可以正确的被编译。

###### Todo List
 
- [ ] Ape/brain/cortex/memory，待添加percepton net，mind层的逻辑网络。
- [ ] Ape/brann/Cortex/instinct/mind/intuition，做为mind的基层部分，待实现。
- [ ] Ape/Output待实现。
- [ ] 逻辑推理只在mind的最高级活动实现，unconscious层只有flex，没回逻辑矢量。

###### Issues

- [ ] 此版是一个重构的中间版本，没有经过基本的run-time的测试验证。

###### Conception

**2018年7月17日，晴热**  
这几口想法又发生了改变：不在unconscious的部分实现田逻辑矢量，但要添加二元组，或三元组，做为unconscious的基本抽象单元。

##### V1.0.0

初始版本。主要是实现Ape的input和brain/cortex/instinct这两个核心部件。Instinct和memory是mind的前，已实现cortex和instinct之间bug msg的相互通信（最基本的消息机制）。

###### Feature List

- [x] 实现了主动类的基类`class nsAI::nsNeuron::CActiveTarget`
- [x] 实现了ape/brain/cortex/instinct。
- [x] 实现了ape/input
- [x] 实现了ape/neuron/bus_server
- [x] 实现了ape/neuron/bus/message
- [x] 实现了ape/neuron/bus/txtmsg



###### Todo List

- [ ] Ape/brain/cortex/memory待实现。
- [ ] Ape/brann/Cortex/instinct/mind待实现。
- [ ] Ape/Output待实现。
- [ ] AI/TSMsgQueue待隔离出来。
- [ ] Debug log的方法。

###### Issues

- [ ] ape工程编译时缺少头文件
> main.cpp(4): fatal error C1083: 无法打开包括文件: “stdafx.h”: No such file or directory
###### Conception

+ Env
    - Earth sphere
    - Another Ape
+ Minding Logic
    - Associative net in the mind
    - Create inspiration
    - Instinct scanning
+ Evolute


