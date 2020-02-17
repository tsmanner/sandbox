#include <bitset>
#include <iostream>
#include <iomanip>
#include <limits>
#include <type_traits>
#include <unordered_set>
using std::cout;
using std::dec;
using std::endl;
using std::hex;
using std::setw;
using std::setfill;
using std::unordered_set;

#include "ArrayContent.h"
#include "ArrayField.h"
#include "ArrayFields.h"
#include "ArraySubFields.h"
#include "../BitManipulation/Interleave.h"


int main() {
  using Entry = ArrayFields<
    ArrayField<0, 3>,
    ArrayField<4 ,7>
    // ArrayField<7, 15>,
    // ArrayField<16, 19>
  >;

  using ArrayType = BufferedArrayContent<
    ArrayFields<
      ArrayField<0>,
      ArraySubFields< 1,  8, Entry>,
      ArraySubFields< 9, 16, Entry>,
      ArraySubFields<17, 24, Entry>
    >//,
    // Interleave<0, 15, 2>
  >;

  auto array = ArrayType();
  // array.setContent(0b10101010101010101010);

  cout
    << "ArrayType<"
    << ArrayType::MSB
    << ", "
    << ArrayType::LSB
    << ">"
    << endl
    << "  Size: " << sizeof(ArrayType::DataType) << endl
    << "  Width: " << ArrayType::WIDTH << endl
    << "  NumFields: " << ArrayType::NumFields << endl
    << hex
    << "  mask<0>: " << ArrayType::calculate_mask<0>() << endl
    << "  mask<1>: " << ArrayType::calculate_mask<1>() << endl
    // << "  mask<2>: " << ArrayType::calculate_mask<2>() << endl
    // << "  mask<3>: " << ArrayType::calculate_mask<3>() << endl
    ;

  cout << hex << Entry(1, 1).getContent() << endl;

  array = ArrayType(1, Entry(1, 1), Entry::calculate_content(0, 0), Entry::calculate_content(1, 1));
  array.setField<1>(Entry::calculate_content(2, 2));
  cout
    << "ArrayInstance.mContent = 0x"
    << hex << setfill('0')
    << setw((ArrayType::WIDTH/4) + ((ArrayType::WIDTH % 4) ? 1 : 0))
    << array.getContent()
    << dec << setfill(' ')
    << endl
    << "ArrayInstance.getField<0>() = "
    << hex
    << +array.getField<0>()
    << endl
    << "ArrayInstance.getField<1>().getField<0>() = "
    << hex
    << +array.getField<1>().getField<0>()
    << endl
    ;

  // array.set(
  //   0b11111,      // Field 0 - < 2: 6>
  //   0b11,         // Field 1 - < 0: 1>
  //   0b100000000,  // Field 2 - < 7:15>
  //   0b0000        // Field 3 - <16:19>
  // );

  // cout
  //   << "query< 0>: " << ArrayType::query< 0>() << endl
  //   << "query< 1>: " << ArrayType::query< 1>() << endl
  //   << "query< 2>: " << ArrayType::query< 2>() << endl
  //   << "query< 3>: " << ArrayType::query< 3>() << endl
  //   << "query< 4>: " << ArrayType::query< 4>() << endl
  //   << "query< 5>: " << ArrayType::query< 5>() << endl
  //   << "query< 6>: " << ArrayType::query< 6>() << endl
  //   << "query< 7>: " << ArrayType::query< 7>() << endl
  //   << "query< 8>: " << ArrayType::query< 8>() << endl
  //   << "query< 9>: " << ArrayType::query< 9>() << endl
  //   << "query<10>: " << ArrayType::query<10>() << endl
  //   << "query<11>: " << ArrayType::query<11>() << endl
  //   << "query<12>: " << ArrayType::query<12>() << endl
  //   << "query<13>: " << ArrayType::query<13>() << endl
  //   << "query<14>: " << ArrayType::query<14>() << endl
  //   << "query<15>: " << ArrayType::query<15>() << endl
  //   << "query<16>: " << ArrayType::query<16>() << endl
  //   << "query<17>: " << ArrayType::query<17>() << endl
  //   << "query<18>: " << ArrayType::query<18>() << endl
  //   << "query<19>: " << ArrayType::query<19>() << endl
  //   << endl;

  // cout
  //   << "ArrayInstance.getContent   = 0b"
  //   << hex << setfill('0')
  //   << std::bitset<20>(array.getContent())
  //   << dec << setfill(' ')
  //   << endl;

  // cout
  //   << "ArrayInstance.getScrambled = 0b"
  //   << hex << setfill('0')
  //   << std::bitset<20>(array.getScrambledContent())
  //   << dec << setfill(' ')
  //   << endl;

  // cout
  //   << hex
  //   << "ArrayInstance.getField<0> = " << array.getField<0>() << endl
  //   << "ArrayInstance.getField<1> = " << array.getField<1>() << endl
  //   << "ArrayInstance.getField<2> = " << array.getField<2>() << endl
  //   << "ArrayInstance.getField<3> = " << array.getField<3>() << endl
  //   ;

  // cout << endl << "ArrayInstance.setField<0>(0x1ff)" << endl;
  // array.setField<2>(0x1f0);
  // cout
  //   << "ArrayInstance.getContent   = 0b"
  //   << hex << setfill('0')
  //   << std::bitset<20>(array.getContent())
  //   << dec << setfill(' ')
  //   << endl;

  // cout
  //   << "ArrayInstance.getScrambled = 0b"
  //   << hex << setfill('0')
  //   << std::bitset<20>(array.getScrambledContent())
  //   << dec << setfill(' ')
  //   << endl;

  // cout
  //   << "ArrayInstance.unscramble   = 0b"
  //   << hex << setfill('0')
  //   << std::bitset<20>(ArrayType::unscramble(array.getScrambledContent()))
  //   << dec << setfill(' ')
  //   << endl;

  // cout
  //   << hex
  //   << "ArrayInstance.getField<0> = " << array.getField<0>() << endl
  //   << "ArrayInstance.getField<1> = " << array.getField<1>() << endl
  //   << "ArrayInstance.getField<2> = " << array.getField<2>() << endl
  //   << "ArrayInstance.getField<3> = " << array.getField<3>() << endl
  //   ;

}
