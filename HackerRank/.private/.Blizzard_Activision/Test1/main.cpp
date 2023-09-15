#include <limits>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

/** Add new members if necessary. */

class MovingAverage
{
public:
    MovingAverage();
    ~MovingAverage();
    
    void AddSample(int sample);
    int GetAverage() const;
    
private:
    enum { W = 10 };
    long long total;
    long long count;
    size_t l10_index = 0;
    int last_10_samples[W]; // circular queue
    long long last_10_total = 0;
};

/** Fill in implementation below. */

MovingAverage::MovingAverage()
{
    total = 0L;
    count = 0;
    last_10_total = 0L;
    for(auto i=0; i < W; ++i)
    {
        last_10_samples[i] = 0;
    }
    l10_index = 0;
}

MovingAverage::~MovingAverage()
{
}

void MovingAverage::AddSample(int sample)
{
    // Assumption: Though the exmaple says hex, the reading logic in main()
    // is just reading in as int, and the signature of this method is void(int)
    // hence, for now, I will assume it's int rather than HEX (string bytes) which
    // I'd have to convert it down to decimal
    
    
    // after re-reading, we want average of the PREVIOUS W(10) samples
    // and my assumptions is also the fact that if there are less than
    // W inputs, it's based on that count, so if there are only 1
    // input, and input was 7, then 7/1 = 7...
    // add new value to the circular queue:
    if((l10_index + 1) >= W) {
        l10_index = 0;
    }
    else {
        ++l10_index;
    }
    last_10_samples[l10_index] = sample;
    
    // NOTE: Because of the comment that AddSample() will
    // be called more than GetAverage(), ideally, we'd want
    // to just add new sample to circular queue here
    // and do the calculation of summing/iterating
    // through the array in GetAverage(), but due to
    // the interface/signature of GetAverage being
    // const, we'd have to do it other ways...
    // probably most optimal is to remember last
    // average, subtrace what gets removed, and add
    // new sample to get total...  if time is available,
    // will do that... but for now, I'll just iterate
    // through the array since it's only 10 int32's
    
    // now sum the total of last 10 samples
    int last_index = W;
    auto start_index = 0;
    if(count < W)
    {
        last_index = (int)l10_index + 1;
    }
    last_10_total = 0;
    for(auto i = start_index; i < last_index; ++i)
    {
        last_10_total += last_10_samples[i];
    }
    
    // just in case, we'll preserve count (useful for checking if circular queue has wrapped or not)
    total += sample;    // unused now, but will track it for now...
    ++count ;
}

// Sample:
// 08 - 08
// 08 0A - 09
// 08 0A 06 - 08
// 08 0A 06 08 - 08
// 08 0A 06 08 07 - 07
// 08 0A 06 08 07 09 - 08
// 08 0A 06 08 07 09 08 - 08
// 08 0A 06 08 07 09 08 12 - 09
// 08 0A 06 08 07 09 08 12 14 - 0A
// 08 0A 06 08 07 09 08 12 14 13 - 0B
// 15 0A 06 08 07 09 08 12 14 13 - 0C - note that from here on, we shift
// 15 12 06 08 07 09 08 12 14 13 - 0D 
int MovingAverage::GetAverage() const
{
    // edge-case to avoid div-by-zero
    if (count == 0)
    { 
        return 0;
    }
    
    //return total / count;
    auto c = count > W ? W : count;
    return last_10_total / c;
}
int main()
{
    int numValues = 0;
    cin >> numValues;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ofstream fout(getenv("OUTPUT_PATH"));
    
    MovingAverage average;
    for(int i = 0; i < numValues; i++) {
        int value;
        cin >> value;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        average.AddSample(value);
        fout << average.GetAverage() << endl;
    }

    fout.close();
    return 0;
}