# Sequence Alignment

Implementation of both global ([Needleman-Wunsch](https://en.wikipedia.org/wiki/Needleman%E2%80%93Wunsch_algorithm)) and local ([Smith-Waterman](https://en.wikipedia.org/wiki/Smith%E2%80%93Waterman_algorithm)) alignment algorithms with naive and affine gap penalties.

## Primer

In bioinformatics, a **sequence alignment** is a way of arranging the sequences of DNA, RNA, or protein to identify regions of similarity that may be a consequence of functional, structural, or evolutionary **relationships** between the sequences.

**Needleman–Wunsch** (global alignment) algorithm was one of the first applications of dynamic programming to compare biological sequences. The algorithm essentially divides a large problem (e.g. the full sequence) into a series of smaller problems, and it uses the solutions to the smaller problems to find an optimal solution to the larger problem. The algorithm assigns a **score** to every possible alignment, and the purpose of the algorithm is to find the alignment having the highest score.

The **Smith–Waterman** algorithm performs local sequence alignment; that is, for determining similar regions between two strings of nucleic acid sequences or protein sequences. Instead of looking at the entire sequence, the Smith–Waterman algorithm compares segments of all possible lengths and optimizes the similarity measure. The main difference to the **Needleman–Wunsch** algorithm is that negative scoring matrix cells are set to zero, which renders the (thus positively scoring) local alignments visible. Traceback procedure starts at the highest scoring matrix cell and proceeds until a cell with score zero is encountered, yielding the highest scoring local alignment. 

## Installation

```bash
$ git clone https://github.com/salmoor/sequence-alignment.git
$ cd sequence-alignment
$ make
```

## How to use

Program can run in 4 different modes.

One can call the program in the following ways:

```bash
$ ./allalign --mode global --input sequences.fasta --gapopen -5
$ ./allalign --mode aglobal --input sequences.fasta --gapopen -5 --gapext -2
$ ./allalign --mode local --input sequences.fasta --gapopen -5
$ ./allalign --mode alocal --input sequences.fasta --gapopen -5 --gapext -2
```

Input parameters are as follows:

- **−−mode:** It will be selected from one of the followings:
  - *global:* Needleman-Wunsch with naive gap scoring
  - *local:* Smith-Waterman with naive gap scoring
  - *aglobal:* Needleman-Wunsch with affine gap scoring
  - *alocal:* Smith-Waterman with affine gap scoring
- **−−input:** Input FASTA file for sequences
- **−−gapopen:** Gap opening penalty for affine gap model, or unit gap cost for naive model
- **−−gapext:** Gap extension penalty for affine gap model



**Output:**

Final alignment along with the score is returned in

- **global-naiveGap.aln** if mode is **global**
- **global-affineGap.aln** if mode is **aglobal**
- **local-naiveGap.aln** if mode is **local**
- **local-affineGap.aln** if mode is **alocal**

