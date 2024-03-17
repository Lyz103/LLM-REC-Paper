# Graph-Mamba:TowardsLong-RangeGraphSequenceModeling with Selective State Spaces

##  Abstract

注意机制已广泛应用于图变换中，以捕捉节点之间的长程依赖关系。由于二次计算成本的瓶颈，注意机制在大型图中无法扩展。最近在计算效率方面的改进主要通过采用带有随机或基于启发式的图子采样的注意力稀疏化来实现，但这在数据相关的上下文推理方面存在不足。状态空间模型（SSM），如Mamba，在建模序列数据中的长程依赖性方面取得了显著进展，因其在序列数据中的长程依赖性方面的有效性和效率而备受推崇。然而，将SSMs调整为非序列图数据面临着显著挑战。在这项工作中，我们介绍了Graph Mamba，这是首次尝试通过集成Mamba块和输入相关节点选择机制来增强图网络中的长程上下文建模。具体而言，我们制定了以图为中心的节点优先级和排列策略，以增强上下文感知推理，从而在预测性能方面取得了显著的改进。对十个基准数据集的广泛实验证明，Graph Mamba在长程图预测任务中优于最先进的方法，且在FLOPs和GPU内存消耗方面只需一小部分计算成本。代码和模型公开可用，地址为https://github.com/bowang-lab/Graph-Mamba。

## Introduction

图建模已被广泛应用于处理复杂的数据结构和关系，例如社交网络（Fan等，2019），分子相互作用（Tsepa等，2023）和脑连接（Li等，2021）。最近，由于它们在建模节点之间的长程连接方面具有强大的能力，图变换器越来越受欢迎（Yun等，2019；Dwivedi＆Bresson，2012；Kreuzer等，2021a；Chen等，2022）。典型的图变换器注意机制允许图中的每个节点与所有其他节点进行交互（Vaswani等，2017）。这作为本地消息传递方法的补充，后者主要编码基于边的邻域拓扑结构（Kipf＆Welling，2016；Xu等，2018）。为了简化图变换器的构建，GraphGPS设计了一个统一的框架，结合了注意模块，消息传递神经网络（MPNN）和结构/位置编码（SE/PE）。这些组件共同更新节点和边的嵌入，用于下游预测任务。这样的解耦流程为用户提供了极大的灵活性，以以插拔方式集成各种注意模块（Ramp´ aˇ sek等，2022）。

虽然Transformers展示了显著的建模能力提升，但由于注意机制带来的二次计算成本，它们在处理长序列时受到了阻碍。这个限制促使了对线性时间注意方法的进一步研究。例如，BigBird（Zaheer等，2020）和Performer（Choromanski等，2020）尝试用稀疏注意或低维矩阵来近似完整的注意力。然而，由于BigBird设计用于序列输入，它在处理非序列输入（如图形）时泛化能力较差，在GraphGPS中导致性能下降（Shirzad等，2023）。Exphormer将这样的注意力稀疏化原则调整为图输入，通过将由边定义的本地连接性作为局部注意力（Shirzad等，2023）。这些调整导致了与完整图注意力相媲美的性能改善。

然而，近似完整的注意力或编码所有上下文可能并非对于长程依赖关系是理想的。根据经验观察，许多序列模型在上下文长度增加时并未取得改进（Gu＆Dao，2023）。Mamba是一种选择性状态空间模型（SSM），旨在解决序列建模中的数据相关上下文压缩问题（Gu＆Dao，2023）。与注意力计算不同，Mamba继承了状态空间模型的构造，通过循环扫描期间使用隐藏状态来编码上下文。选择机制允许控制哪部分输入可以流入隐藏状态，作为影响后续嵌入更新的上下文的一部分。在图建模中，这可以看作是一个数据相关的节点选择过程。通过在每个循环步骤中过滤相关节点并仅“关注”选定的上下文，Mamba有助于实现与注意力稀疏化相同的目标，是随机子采样的替代方法。此外，Mamba模块经过优化，具有线性时间复杂度和减少内存的特性，为大型图训练任务提供了改进的效率。然而，在有效地将为序列建模而设计的Mamba适应到非序列图输入中存在挑战。

贡献。受Mamba卓越的长序列建模能力的启发，我们提出了Graph-Mamba，以减轻与图变换器相关的高计算成本，作为注意力稀疏化的数据相关替代方案。具体而言，这项工作提出了以下关键贡献：

• 创新的图网络设计：Graph-Mamba代表了一种新型的图网络，是与选择性状态空间模型集成的先驱。它执行基于输入的节点过滤和自适应上下文选择。选择机制捕捉长程依赖关系，并改进了现有的基于子采样的注意力稀疏化技术。

• 非序列图数据中SSMs的调整：我们设计了一种优雅的方式，将状态空间模型扩展到处理非序列图数据。具体而言，我们引入了一种节点优先级技术，以优先考虑重要节点以获得更多的上下文访问，并采用基于排列的训练方案以最小化与序列相关的偏差。

• 卓越的性能和效率：对十个公共数据集的全面实验证明，Graph-Mamba不仅优于基线，而且实现了线性时间的计算复杂度。值得注意的是，Graph-Mamba在大型图上将GPU内存消耗降低了最多74%，突显了其在长程图数据集中的高效性。

## Related Work

 **2.1. Graph Neural Networks**

图神经网络（GNNs）利用消息传递作为图建模的关键机制，使节点能够与其邻居进行通信并迭代地聚合信息。图卷积网络（GCN）（Kipf＆Welling，2017；Defferrard等，2017）是GNNs的先驱，影响了后续的工作，如Graph Sage（Hamilton等，2018），GIN（Xu等，2018），GAT（Veliˇckovi´c等，2018）和GatedGCN（Bresson＆Laurent，2018）。尽管在基于图拓扑的基础上聚合节点特征具有重要意义，但MPNNs的表达能力受到了1维Weisfeiler-Lehman（1-WL）同构测试的限制（Xu等，2018）。此外，聚合的节点特征容易在局部邻域中过度平滑（Alon＆Yahav，2021；Topping等，2022）。

**2.2. Graph Transformers**

注意机制的Transformer在各个领域取得了前所未有的成功，包括自然语言处理（NLP）（Vaswani等，2017；Kalyan等，2021）和计算机视觉（D'Ascoli等，2021；Guo等，2022；Dosovitskiy等，2021）。图变换器通常计算完整的注意力，允许每个节点关注所有其他节点，而不考虑边的连接性。这使得图变换器能够有效捕捉长程依赖关系，避免了在局部邻域中像MPNNs一样的过度聚合。然而，完整的注意力由于其O(N^2)的复杂性，在大型图中无法扩展。

类似于NLP中transformers中的位置嵌入，第一个图变换器（Dwivedi＆Bresson，2021）将图拉普拉斯特征向量引入节点位置嵌入。随后，SAN（Kreuzer等，2021b）整合了不变的位置嵌入聚合，为真实和虚拟图边都整合了条件注意力。与此同时，Graphormer（Ying等，2021）利用中心性和空间编码将相对位置嵌入整合到注意力机制中。GraphiT（Mialon等，2021）利用基于扩散核的相对位置嵌入来模拟注意机制。最后，GraphTrans（Wu等，2022）提出了一个两阶段的架构，使用从MPNNs派生的局部嵌入上的图变换器。

**2.3. GraphGPS**

GraphGPS（Ramp´aˇsek等，2022）采用了一个模块化框架，集成了SE、PE、MPNN和图变换器。用户可以在这个框架内灵活选择每个组件的方法。给定一个输入图，GraphGPS计算SE和PE，将它们与节点和边的嵌入连接起来，然后将这些嵌入传递到GPS层。在GPS层中，图变换器和MPNN共同更新节点和边的嵌入。GraphGPS框架允许将完全连接的Transformer注意力替换为其稀疏替代方案，从而实现O(N + E)的复杂性。

 **2.4. Sparse Graph Attention**

BigBird（Zaheer等，2020）和Performer（Choromanski等，2020）是GraphGPS支持的两种稀疏注意方法。Performer通过使用低维正交随机特征来近似常规注意力中的softmax核，从而提高了计算效率。BigBird采用图子采样和序列启发式来近似完整的注意力，结合了随机子采样的注意力、相邻标记之间的局部注意力以及与全局标记相关的全局注意力（Zaheer等，2020）。已知随机子采样图，或扩展器，能够近似完整图的谱特性（Spielman＆Teng，2011；Yun等，2020）。然而，BigBird的局部注意力是专为具有相邻标记上滑动窗口的序列输入而设计的，因此不适合建模图输入。Exphormer（Shirzad等，2023）通过在由边定义的邻居之间引入局部邻域注意力以及将虚拟节点与扩展器中的所有节点连接的全局注意力，提出了BigBird的图适应版本。这些调整进一步改善了模型性能，同时受益于稀疏注意力的线性复杂性。然而，随机节点子采样过程表明可能存在改进的空间。整合能够允许有信息的上下文选择的方法可能进一步提升模型性能。

 **2.5. State Space Models**

一般的状态空间模型涉及通过隐藏状态对序列进行反复更新。实现范围从隐藏马尔可夫模型到深度学习中的循环神经网络（RNNs）。利用循环扫描，SSM在其隐藏状态中存储上下文，并通过将这些隐藏状态与输入组合来更新输出。结构化状态空间模型（S4）通过重新参数化（Gu等，2021）提高了计算效率，为注意力计算提供了一种有效的替代方案。用于线性时间注意的最近的S4变体包括H3（Fu等，2022），Gated State Space（Mehta等，2022），Hyena（Nguyen等，2023）和RWKV（Peng等，2023）。Mamba进一步向S4引入了一个数据相关的选择机制，以捕捉随着序列长度增加的长程上下文（Gu＆Dao，2023）。值得注意的是，Mamba在长序列建模中展现了线性的效率，并在各种基准测试中优于Transformers。Mamba还成功地适应了非序列输入，例如在分割任务中对图像进行增强长程依赖性（Ma等，2024；Zhu等，2024；Liu等，2024）。

## Graph-Mamba

Graph-Mamba采用了一种选择性SSM来实现依赖输入的图稀疏化。具体而言，我们设计了一个Graph-Mamba块（GMB）并将其整合到流行的GraphGPS框架中，使其能够与其他图注意实现进行公平比较。GMB利用了序列建模中的循环扫描，结合选择机制实现了两个级别的图稀疏化。第一级涉及Mamba模块中的选择机制，它有效地过滤了长程上下文中的相关信息。第二级通过提出的节点优先级方法实现，允许图中的重要节点访问更多上下文。因此，这些序列建模特征为结合数据相关和启发式通知选择图稀疏化提供了一个有前途的途径。此外，使用Mamba模块的Graph-Mamba实现确保了线性时间复杂度，作为密集图注意力的有效替代方案。

为了在图建模中将SSMs置于上下文中，我们首先在第3.1和3.2节中回顾SSMs，然后讨论选择机制。接下来，在第3.3节中介绍Graph-Mamba的实现，然后在第3.4和3.5节详细说明GMB的专业图适应技术。最后，在第3.6节中讨论GMB的计算效率。