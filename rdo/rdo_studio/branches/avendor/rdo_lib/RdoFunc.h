#ifndef RDO_FUNC_H_
#define RDO_FUNC_H_

class RandGenerator
{
protected:
   unsigned int seed;
	RandGenerator(unsigned int i_seed = 123456789): seed(i_seed) {}
public:
	void setSeed(unsigned int i_seed = 123456789) { seed = i_seed; }
	virtual ~RandGenerator() {};
};

class RandGeneratorUniform: public RandGenerator
{
public:
   RandGeneratorUniform(unsigned int i_seed = 123456789): RandGenerator(i_seed) {}
   double next(double from, double to)
   {
      seed = seed * 69069 + 1;
      return seed / ((long double)0xFFFFFFFF + 1) * (to-from) + from;
   }

};

class RandGeneratorExponential: public RandGeneratorUniform
{
public:
   RandGeneratorExponential(unsigned int i_seed = 123456789): RandGeneratorUniform(i_seed) {}
   double next(double math)
   {
      double ran = RandGeneratorUniform::next(0, 1);
      double res = -log(ran) * math;
      return res;
   }
};

class RandGeneratorNormal: public RandGeneratorUniform
{
public:
   RandGeneratorNormal(unsigned int i_seed = 123456789): RandGeneratorUniform(i_seed) {}
   double next(double av, double var)
   {
      double ran = 0;
      for(int i = 0; i < 12; i++)
         ran += RandGeneratorUniform::next(0, 1);

      double res = var * (ran - 6) + av;
      return res;
   }
};

class RandGeneratorByHistCommon: public RandGeneratorUniform
{
protected:
   double summ;
   RandGeneratorByHistCommon(unsigned int i_seed = 123456789): RandGeneratorUniform(i_seed) {}
public:
   virtual double next() = 0;
};

class RandGeneratorByHist: public RandGeneratorByHistCommon
{
   std::vector<double> from;
   std::vector<double> to;
   std::vector<double> freq;
public:
   RandGeneratorByHist(unsigned int i_seed = 123456789): RandGeneratorByHistCommon(i_seed) {}
   void setValues(int num, ...)
   {
      va_list ap;
      va_start(ap, num);
      from.clear();
      to.clear();
      freq.clear();
      for(int i =0; i < num; i++)
      {
         from.push_back(va_arg(ap, double));
         to.push_back(va_arg(ap, double));
         freq.push_back(va_arg(ap, double));
      }

      va_end(ap);

      summ = 0;
      for(i = 0; i < freq.size(); i++)
         summ += freq[i];
   }

   void addValues(double _from, double _to, double _freq)
   {
      from.push_back(_from);
      to.push_back(_to);
      freq.push_back(_freq);

      summ = 0;
      for(int i = 0; i < freq.size(); i++)
         summ += freq[i];
   }

   double next()
   {
      double ran1 = RandGeneratorUniform::next(0, summ);
      double add = 0;
      for(int i = 0; i < freq.size() - 1; i++)
      {
         if(ran1 < freq[i] + add)
            break;

         add += freq[i];
      }

      return RandGeneratorUniform::next(from[i], to[i]);
   }
};

class RandGeneratorByHistEnum: public RandGeneratorByHistCommon
{
   std::vector<int> vals;
   std::vector<double> freq;
public:
   RandGeneratorByHistEnum(unsigned int i_seed = 123456789): RandGeneratorByHistCommon(i_seed) {}
   void setValues(int num, ...)
   {
      va_list ap;
      va_start(ap, num);
      vals.clear();
      freq.clear();
      for(int i =0; i < num; i++)
      {
         vals.push_back(va_arg(ap, int));
         freq.push_back(va_arg(ap, double));
      }

      va_end(ap);

      summ = 0;
      for(i = 0; i < freq.size(); i++)
         summ += freq[i];
   }

   void addValues(int _val, double _freq)
   {
      vals.push_back(_val);
      freq.push_back(_freq);

      summ = 0;
      for(int i = 0; i < freq.size(); i++)
         summ += freq[i];
   }

   double next()
   {
      double ran1 = RandGeneratorUniform::next(0, summ);
      double add = 0;
      for(int i = 0; i < freq.size() - 1; i++)
      {
         if(ran1 < freq[i] + add)
            break;

         add += freq[i];
      }

      return vals[i];
   }
};


class RandGeneratorEnumerative: public RandGeneratorByHistCommon
{
   std::vector<double> vals;
   int curr;
public:
   void setValues(int num, ...)
   {
      va_list ap;
      va_start(ap, num);
      vals.clear();
      for(int i =0; i < num; i++)
         vals.push_back(va_arg(ap, double));

      va_end(ap);
      curr = 0;
   }

   void addValue(double val)
   {
      vals.push_back(val);
      curr = 0;
   }

   double next()
   {
      double res = vals[curr++];
      if(curr >= vals.size())
         curr = 0;

      return res;
   }
};

#endif //RDO_FUNC_H_
