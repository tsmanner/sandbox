class SpecVal {
public:
  SpecVal() {}
  SpecVal(const int& value, const bool& concrete = true): mIsConcrete(concrete), mValue(value) {}

  const SpecVal& operator=(const int& value) {
    mValue = value;
    return *this;
  }

  operator int() const {
    if (mIsConcrete) return mValue;
    return -1;
  }

  void makeConcrete() { mIsConcrete = true; }

private:
  bool mIsConcrete = false;
  int mValue = 0;

};
