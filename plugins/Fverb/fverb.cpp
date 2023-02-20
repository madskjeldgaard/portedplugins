/* ------------------------------------------------------------
author: "Jean Pierre Cimalando"
license: "BSD-2-Clause"
name: "fverb"
version: "0.5"
Code generated with Faust 2.54.9 (https://faust.grame.fr)
Compilation options: -a supercollider.cpp -lang cpp -i -es 1 -mcd 16 -single
-ftz 0
------------------------------------------------------------ */

#ifndef __mydsp_H__
#define __mydsp_H__

/************************************************************************
 IMPORTANT NOTE : this file contains two clearly delimited sections :
 the ARCHITECTURE section (in two parts) and the USER section. Each section
 is governed by its own copyright and license. Please check individually
 each section for license and copyright information.
 *************************************************************************/

/******************* BEGIN supercollider.cpp ****************/
/************************************************************************
 FAUST Architecture File
 Copyright (C) 2005-2012 Stefan Kersten.
 Copyright (C) 2003-2019 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.

 ************************************************************************
 ************************************************************************/

// The prefix is set to "Faust" in the faust2supercollider script, otherwise set
// empty
#if !defined(SC_FAUST_PREFIX)
#define SC_FAUST_PREFIX ""
#endif

#include <SC_PlugIn.h>
#include <string.h>

#include <map>
#include <string>

/************************** BEGIN dsp.h ********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef __export__
#define __export__

#define FAUSTVERSION "2.54.9"

// Use FAUST_API for code that is part of the external API but is also compiled
// in faust and libfaust Use LIBFAUST_API for code that is compiled in faust and
// libfaust

#ifdef _WIN32
#pragma warning(disable : 4251)
#ifdef FAUST_EXE
#define FAUST_API
#define LIBFAUST_API
#elif FAUST_LIB
#define FAUST_API __declspec(dllexport)
#define LIBFAUST_API __declspec(dllexport)
#else
#define FAUST_API
#define LIBFAUST_API
#endif
#else
#ifdef FAUST_EXE
#define FAUST_API
#define LIBFAUST_API
#else
#define FAUST_API __attribute__((visibility("default")))
#define LIBFAUST_API __attribute__((visibility("default")))
#endif
#endif

#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct FAUST_API UI;
struct FAUST_API Meta;

/**
 * DSP memory manager.
 */

struct FAUST_API dsp_memory_manager {
  virtual ~dsp_memory_manager() {}

  /**
   * Inform the Memory Manager with the number of expected memory zones.
   * @param count - the number of expected memory zones
   */
  virtual void begin(size_t /*count*/) {}

  /**
   * Give the Memory Manager information on a given memory zone.
   * @param size - the size in bytes of the memory zone
   * @param reads - the number of Read access to the zone used to compute one
   * frame
   * @param writes - the number of Write access to the zone used to compute one
   * frame
   */
  virtual void info(size_t /*size*/, size_t /*reads*/, size_t /*writes*/) {}

  /**
   * Inform the Memory Manager that all memory zones have been described,
   * to possibly start a 'compute the best allocation strategy' step.
   */
  virtual void end() {}

  /**
   * Allocate a memory zone.
   * @param size - the memory zone size in bytes
   */
  virtual void* allocate(size_t size) = 0;

  /**
   * Destroy a memory zone.
   * @param ptr - the memory zone pointer to be deallocated
   */
  virtual void destroy(void* ptr) = 0;
};

/**
 * Signal processor definition.
 */

class FAUST_API dsp {
 public:
  dsp() {}
  virtual ~dsp() {}

  /* Return instance number of audio inputs */
  virtual int getNumInputs() = 0;

  /* Return instance number of audio outputs */
  virtual int getNumOutputs() = 0;

  /**
   * Trigger the ui_interface parameter with instance specific calls
   * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the
   * UI.
   *
   * @param ui_interface - the user interface builder
   */
  virtual void buildUserInterface(UI* ui_interface) = 0;

  /* Return the sample rate currently used by the instance */
  virtual int getSampleRate() = 0;

  /**
   * Global init, calls the following methods:
   * - static class 'classInit': static tables initialization
   * - 'instanceInit': constants and instance state initialization
   *
   * @param sample_rate - the sampling rate in Hz
   */
  virtual void init(int sample_rate) = 0;

  /**
   * Init instance state
   *
   * @param sample_rate - the sampling rate in Hz
   */
  virtual void instanceInit(int sample_rate) = 0;

  /**
   * Init instance constant state
   *
   * @param sample_rate - the sampling rate in Hz
   */
  virtual void instanceConstants(int sample_rate) = 0;

  /* Init default control parameters values */
  virtual void instanceResetUserInterface() = 0;

  /* Init instance state (like delay lines...) but keep the control parameter
   * values */
  virtual void instanceClear() = 0;

  /**
   * Return a clone of the instance.
   *
   * @return a copy of the instance on success, otherwise a null pointer.
   */
  virtual dsp* clone() = 0;

  /**
   * Trigger the Meta* parameter with instance specific calls to 'declare' (key,
   * value) metadata.
   *
   * @param m - the Meta* meta user
   */
  virtual void metadata(Meta* m) = 0;

  /**
   * DSP instance computation, to be called with successive in/out audio
   * buffers.
   *
   * @param count - the number of frames to compute
   * @param inputs - the input audio buffers as an array of non-interleaved
   * FAUSTFLOAT samples (eiher float, double or quad)
   * @param outputs - the output audio buffers as an array of non-interleaved
   * FAUSTFLOAT samples (eiher float, double or quad)
   *
   */
  virtual void compute(int count, FAUSTFLOAT** inputs,
                       FAUSTFLOAT** outputs) = 0;

  /**
   * DSP instance computation: alternative method to be used by subclasses.
   *
   * @param date_usec - the timestamp in microsec given by audio driver.
   * @param count - the number of frames to compute
   * @param inputs - the input audio buffers as an array of non-interleaved
   * FAUSTFLOAT samples (either float, double or quad)
   * @param outputs - the output audio buffers as an array of non-interleaved
   * FAUSTFLOAT samples (either float, double or quad)
   *
   */
  virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs,
                       FAUSTFLOAT** outputs) {
    compute(count, inputs, outputs);
  }
};

/**
 * Generic DSP decorator.
 */

class FAUST_API decorator_dsp : public dsp {
 protected:
  dsp* fDSP;

 public:
  decorator_dsp(dsp* dsp = nullptr) : fDSP(dsp) {}
  virtual ~decorator_dsp() { delete fDSP; }

  virtual int getNumInputs() { return fDSP->getNumInputs(); }
  virtual int getNumOutputs() { return fDSP->getNumOutputs(); }
  virtual void buildUserInterface(UI* ui_interface) {
    fDSP->buildUserInterface(ui_interface);
  }
  virtual int getSampleRate() { return fDSP->getSampleRate(); }
  virtual void init(int sample_rate) { fDSP->init(sample_rate); }
  virtual void instanceInit(int sample_rate) {
    fDSP->instanceInit(sample_rate);
  }
  virtual void instanceConstants(int sample_rate) {
    fDSP->instanceConstants(sample_rate);
  }
  virtual void instanceResetUserInterface() {
    fDSP->instanceResetUserInterface();
  }
  virtual void instanceClear() { fDSP->instanceClear(); }
  virtual decorator_dsp* clone() { return new decorator_dsp(fDSP->clone()); }
  virtual void metadata(Meta* m) { fDSP->metadata(m); }
  // Beware: subclasses usually have to overload the two 'compute' methods
  virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {
    fDSP->compute(count, inputs, outputs);
  }
  virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs,
                       FAUSTFLOAT** outputs) {
    fDSP->compute(date_usec, count, inputs, outputs);
  }
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class FAUST_API dsp_factory {
 protected:
  // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
  virtual ~dsp_factory() {}

 public:
  virtual std::string getName() = 0;
  virtual std::string getSHAKey() = 0;
  virtual std::string getDSPCode() = 0;
  virtual std::string getCompileOptions() = 0;
  virtual std::vector<std::string> getLibraryList() = 0;
  virtual std::vector<std::string> getIncludePathnames() = 0;
  virtual std::vector<std::string> getWarningMessages() = 0;

  virtual dsp* createDSPInstance() = 0;

  virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
  virtual dsp_memory_manager* getMemoryManager() = 0;
};

// Denormal handling

#if defined(__SSE__)
#include <xmmintrin.h>
#endif

class FAUST_API ScopedNoDenormals {
 private:
  intptr_t fpsr = 0;

  void setFpStatusRegister(intptr_t fpsr_aux) noexcept {
#if defined(__arm64__) || defined(__aarch64__)
    asm volatile("msr fpcr, %0" : : "ri"(fpsr_aux));
#elif defined(__SSE__)
    // The volatile keyword here is needed to workaround a bug in
    // AppleClang 13.0 which aggressively optimises away the variable otherwise
    volatile uint32_t fpsr_w = static_cast<uint32_t>(fpsr_aux);
    _mm_setcsr(fpsr_w);
#endif
  }

  void getFpStatusRegister() noexcept {
#if defined(__arm64__) || defined(__aarch64__)
    asm volatile("mrs %0, fpcr" : "=r"(fpsr));
#elif defined(__SSE__)
    fpsr = static_cast<intptr_t>(_mm_getcsr());
#endif
  }

 public:
  ScopedNoDenormals() noexcept {
#if defined(__arm64__) || defined(__aarch64__)
    intptr_t mask = (1 << 24 /* FZ */);
#elif defined(__SSE__)
#if defined(__SSE2__)
    intptr_t mask = 0x8040;
#else
    intptr_t mask = 0x8000;
#endif
#else
    intptr_t mask = 0x0000;
#endif
    getFpStatusRegister();
    setFpStatusRegister(fpsr | mask);
  }

  ~ScopedNoDenormals() noexcept { setFpStatusRegister(fpsr); }
};

#define AVOIDDENORMALS ScopedNoDenormals ftz_scope;

#endif

/************************** END dsp.h **************************/
/************************** BEGIN UI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct FAUST_API UIReal {
  UIReal() {}
  virtual ~UIReal() {}

  // -- widget's layouts

  virtual void openTabBox(const char* label) = 0;
  virtual void openHorizontalBox(const char* label) = 0;
  virtual void openVerticalBox(const char* label) = 0;
  virtual void closeBox() = 0;

  // -- active widgets

  virtual void addButton(const char* label, REAL* zone) = 0;
  virtual void addCheckButton(const char* label, REAL* zone) = 0;
  virtual void addVerticalSlider(const char* label, REAL* zone, REAL init,
                                 REAL min, REAL max, REAL step) = 0;
  virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init,
                                   REAL min, REAL max, REAL step) = 0;
  virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min,
                           REAL max, REAL step) = 0;

  // -- passive widgets

  virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min,
                                     REAL max) = 0;
  virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min,
                                   REAL max) = 0;

  // -- soundfiles

  virtual void addSoundfile(const char* label, const char* filename,
                            Soundfile** sf_zone) = 0;

  // -- metadata declarations

  virtual void declare(REAL* /*zone*/, const char* /*key*/,
                       const char* /*val*/) {}

  // To be used by LLVM client
  virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
  UI() {}
  virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN misc.h *******************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
***************************************************************************/

#ifndef __misc__
#define __misc__

#include <string.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>

/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to
 retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
  virtual ~Meta() {}
  virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/

struct MY_Meta : Meta, std::map<const char*, const char*> {
  void declare(const char* key, const char* value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) {
  int r = 0;
  while ((1 << r) < x) r++;
  return r;
}

static long lopt(char* argv[], const char* name, long def) {
  for (int i = 0; argv[i]; i++)
    if (!strcmp(argv[i], name)) return std::atoi(argv[i + 1]);
  return def;
}

static long lopt1(int argc, char* argv[], const char* longname,
                  const char* shortname, long def) {
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i - 1], shortname) == 0 ||
        strcmp(argv[i - 1], longname) == 0) {
      return atoi(argv[i]);
    }
  }
  return def;
}

static const char* lopts(char* argv[], const char* name, const char* def) {
  for (int i = 0; argv[i]; i++)
    if (!strcmp(argv[i], name)) return argv[i + 1];
  return def;
}

static const char* lopts1(int argc, char* argv[], const char* longname,
                          const char* shortname, const char* def) {
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i - 1], shortname) == 0 ||
        strcmp(argv[i - 1], longname) == 0) {
      return argv[i];
    }
  }
  return def;
}

static bool isopt(char* argv[], const char* name) {
  for (int i = 0; argv[i]; i++)
    if (!strcmp(argv[i], name)) return true;
  return false;
}

static std::string pathToContent(const std::string& path) {
  std::ifstream file(path.c_str(), std::ifstream::binary);

  file.seekg(0, file.end);
  int size = int(file.tellg());
  file.seekg(0, file.beg);

  // And allocate buffer to that a single line can be read...
  char* buffer = new char[size + 1];
  file.read(buffer, size);

  // Terminate the string
  buffer[size] = 0;
  std::string result = buffer;
  file.close();
  delete[] buffer;
  return result;
}

#endif

/**************************  END  misc.h **************************/

using namespace std;

#if defined(__GNUC__) && __GNUC__ >= 4
#define FAUST_EXPORT __attribute__((visibility("default")))
#else
#define FAUST_EXPORT SC_API_EXPORT
#endif

#ifdef WIN32
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

//----------------------------------------------------------------------------
// Metadata
//----------------------------------------------------------------------------

class MetaData : public Meta, public std::map<std::string, std::string> {
 public:
  void declare(const char* key, const char* value) { (*this)[key] = value; }
};

//----------------------------------------------------------------------------
// Control counter
//----------------------------------------------------------------------------

class ControlCounter : public UI {
 public:
  ControlCounter() : mNumControlInputs(0), mNumControlOutputs(0) {}

  size_t getNumControls() const { return getNumControlInputs(); }
  size_t getNumControlInputs() const { return mNumControlInputs; }
  size_t getNumControlOutputs() const { return mNumControlOutputs; }

  // Layout widgets
  virtual void openTabBox(const char* label) {}
  virtual void openHorizontalBox(const char* label) {}
  virtual void openVerticalBox(const char* label) {}
  virtual void closeBox() {}

  // Active widgets
  virtual void addButton(const char* label, FAUSTFLOAT* zone) {
    addControlInput();
  }
  virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {
    addControlInput();
  }
  virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone,
                                 FAUSTFLOAT init, FAUSTFLOAT min,
                                 FAUSTFLOAT max, FAUSTFLOAT step) {
    addControlInput();
  }
  virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone,
                                   FAUSTFLOAT init, FAUSTFLOAT min,
                                   FAUSTFLOAT max, FAUSTFLOAT step) {
    addControlInput();
  }
  virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addControlInput();
  }

  // Passive widgets
  virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone,
                                     FAUSTFLOAT min, FAUSTFLOAT max) {
    addControlOutput();
  }
  virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone,
                                   FAUSTFLOAT min, FAUSTFLOAT max) {
    addControlOutput();
  }

  virtual void addSoundfile(const char* label, const char* filename,
                            Soundfile** sf_zone) {}

 protected:
  void addControlInput() { mNumControlInputs++; }
  void addControlOutput() { mNumControlOutputs++; }

 private:
  size_t mNumControlInputs;
  size_t mNumControlOutputs;
};

//----------------------------------------------------------------------------
// UI control
//----------------------------------------------------------------------------

struct Control {
  typedef void (*UpdateFunction)(Control* self, FAUSTFLOAT value);

  UpdateFunction updateFunction;
  FAUSTFLOAT* zone;
  FAUSTFLOAT min, max;

  inline void update(FAUSTFLOAT value) { (*updateFunction)(this, value); }

  static void simpleUpdate(Control* self, FAUSTFLOAT value) {
    *self->zone = value;
  }
  static void boundedUpdate(Control* self, FAUSTFLOAT value) {
    *self->zone = sc_clip(value, self->min, self->max);
  }
};

//----------------------------------------------------------------------------
// Control allocator
//----------------------------------------------------------------------------

class ControlAllocator : public UI {
 public:
  ControlAllocator(Control* controls) : mControls(controls) {}

  // Layout widgets
  virtual void openTabBox(const char* label) {}
  virtual void openHorizontalBox(const char* label) {}
  virtual void openVerticalBox(const char* label) {}
  virtual void closeBox() {}

  // Active widgets
  virtual void addButton(const char* label, FAUSTFLOAT* zone) {
    addSimpleControl(zone);
  }
  virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {
    addSimpleControl(zone);
  }
  virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone,
                                 FAUSTFLOAT init, FAUSTFLOAT min,
                                 FAUSTFLOAT max, FAUSTFLOAT step) {
    addBoundedControl(zone, min, max, step);
  }
  virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone,
                                   FAUSTFLOAT init, FAUSTFLOAT min,
                                   FAUSTFLOAT max, FAUSTFLOAT step) {
    addBoundedControl(zone, min, max, step);
  }
  virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addBoundedControl(zone, min, max, step);
  }

  // Passive widgets
  virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone,
                                     FAUSTFLOAT min, FAUSTFLOAT max) {}
  virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone,
                                   FAUSTFLOAT min, FAUSTFLOAT max) {}
  virtual void addSoundfile(const char* label, const char* filename,
                            Soundfile** sf_zone) {}

 private:
  void addControl(Control::UpdateFunction updateFunction, FAUSTFLOAT* zone,
                  FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT /* step */) {
    Control* ctrl = mControls++;
    ctrl->updateFunction = updateFunction;
    ctrl->zone = zone;
    ctrl->min = min;
    ctrl->max = max;
  }
  void addSimpleControl(FAUSTFLOAT* zone) {
    addControl(Control::simpleUpdate, zone, 0.f, 0.f, 0.f);
  }
  void addBoundedControl(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max,
                         FAUSTFLOAT step) {
    addControl(Control::boundedUpdate, zone, min, max, step);
  }

 private:
  Control* mControls;
};

/******************************************************************************
 *******************************************************************************

 VECTOR INTRINSICS

 *******************************************************************************
 *******************************************************************************/

/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <math.h>

#include <algorithm>
#include <cmath>
#include <cstdint>

#ifndef FAUSTCLASS
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

class mydspSIG0 {
 private:
  int iVec6[2];
  int iRec34[2];

 public:
  int getNumInputsmydspSIG0() { return 0; }
  int getNumOutputsmydspSIG0() { return 1; }

  void instanceInitmydspSIG0(int sample_rate) {
    for (int l21 = 0; l21 < 2; l21 = l21 + 1) {
      iVec6[l21] = 0;
    }
    for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
      iRec34[l22] = 0;
    }
  }

  void fillmydspSIG0(int count, float* table) {
    for (int i1 = 0; i1 < count; i1 = i1 + 1) {
      iVec6[0] = 1;
      iRec34[0] = (iVec6[1] + iRec34[1]) % 65536;
      table[i1] = std::sin(9.58738e-05f * float(iRec34[0]));
      iVec6[1] = iVec6[0];
      iRec34[1] = iRec34[0];
    }
  }
};

static mydspSIG0* newmydspSIG0() { return (mydspSIG0*)new mydspSIG0(); }
static void deletemydspSIG0(mydspSIG0* dsp) { delete dsp; }

static float ftbl0mydspSIG0[65536];

class mydsp : public dsp {
 private:
  int fSampleRate;
  float fConst0;
  float fConst1;
  float fConst2;
  FAUSTFLOAT fHslider0;
  float fConst3;
  float fRec8[2];
  FAUSTFLOAT fHslider1;
  float fConst4;
  float fRec12[2];
  FAUSTFLOAT fHslider2;
  float fRec15[2];
  FAUSTFLOAT fHslider3;
  float fRec20[2];
  FAUSTFLOAT fHslider4;
  float fRec23[2];
  FAUSTFLOAT fHslider5;
  float fRec25[2];
  FAUSTFLOAT fHslider6;
  float fRec27[2];
  int IOTA0;
  float fVec0[131072];
  FAUSTFLOAT fHslider7;
  float fConst5;
  float fRec28[2];
  float fRec26[2];
  float fRec24[2];
  float fVec1[1024];
  int iConst6;
  float fRec21[2];
  float fVec2[1024];
  int iConst7;
  float fRec18[2];
  float fVec3[4096];
  int iConst8;
  float fRec16[2];
  float fVec4[2048];
  int iConst9;
  float fRec13[2];
  int iConst10;
  FAUSTFLOAT fHslider8;
  float fRec29[2];
  float fVec5[131072];
  FAUSTFLOAT fHslider9;
  float fRec36[2];
  float fRec35[2];
  FAUSTFLOAT fHslider10;
  float fRec37[2];
  float fConst11;
  float fConst12;
  float fRec30[2];
  float fRec31[2];
  int iRec32[2];
  int iRec33[2];
  float fRec10[2];
  float fVec7[32768];
  int iConst13;
  float fRec9[2];
  float fVec8[32768];
  int iConst14;
  float fRec6[2];
  float fRec0[32768];
  float fRec1[16384];
  float fRec2[32768];
  float fVec9[131072];
  float fRec52[2];
  float fRec51[2];
  float fVec10[1024];
  int iConst15;
  float fRec49[2];
  float fVec11[1024];
  int iConst16;
  float fRec47[2];
  float fVec12[4096];
  int iConst17;
  float fRec45[2];
  float fVec13[2048];
  int iConst18;
  float fRec43[2];
  int iConst19;
  float fVec14[131072];
  float fRec53[2];
  float fRec54[2];
  int iRec55[2];
  int iRec56[2];
  float fRec41[2];
  float fVec15[32768];
  int iConst20;
  float fRec40[2];
  float fVec16[16384];
  int iConst21;
  float fRec38[2];
  float fRec3[32768];
  float fRec4[8192];
  float fRec5[32768];
  int iConst22;
  int iConst23;
  int iConst24;
  int iConst25;
  int iConst26;
  int iConst27;
  int iConst28;
  int iConst29;
  int iConst30;
  int iConst31;
  int iConst32;
  int iConst33;
  int iConst34;
  int iConst35;

 public:
  void metadata(Meta* m) {
    m->declare("author", "Jean Pierre Cimalando");
    m->declare("basics.lib/name", "Faust Basic Element Library");
    m->declare("basics.lib/version", "0.9");
    m->declare(
        "compile_options",
        "-a supercollider.cpp -lang cpp -i -es 1 -mcd 16 -single -ftz 0");
    m->declare("delays.lib/name", "Faust Delay Library");
    m->declare("delays.lib/version", "0.1");
    m->declare("filename", "fverb.dsp");
    m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
    m->declare("filters.lib/allpass_comb:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/allpass_comb:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/fir:author", "Julius O. Smith III");
    m->declare("filters.lib/fir:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/iir:author", "Julius O. Smith III");
    m->declare("filters.lib/iir:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/lowpass0_highpass1",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/name", "Faust Filters Library");
    m->declare("filters.lib/version", "0.3");
    m->declare("license", "BSD-2-Clause");
    m->declare("maths.lib/author", "GRAME");
    m->declare("maths.lib/copyright", "GRAME");
    m->declare("maths.lib/license", "LGPL with exception");
    m->declare("maths.lib/name", "Faust Math Library");
    m->declare("maths.lib/version", "2.5");
    m->declare("name", "fverb");
    m->declare("oscillators.lib/name", "Faust Oscillator Library");
    m->declare("oscillators.lib/version", "0.3");
    m->declare("platform.lib/name", "Generic Platform Library");
    m->declare("platform.lib/version", "0.3");
    m->declare("signals.lib/name", "Faust Signal Routing Library");
    m->declare("signals.lib/version", "0.3");
    m->declare("version", "0.5");
  }

  virtual int getNumInputs() { return 2; }
  virtual int getNumOutputs() { return 2; }

  static void classInit(int sample_rate) {
    mydspSIG0* sig0 = newmydspSIG0();
    sig0->instanceInitmydspSIG0(sample_rate);
    sig0->fillmydspSIG0(65536, ftbl0mydspSIG0);
    deletemydspSIG0(sig0);
  }

  virtual void instanceConstants(int sample_rate) {
    fSampleRate = sample_rate;
    fConst0 =
        std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
    fConst1 = 44.1f / fConst0;
    fConst2 = 1.0f - fConst1;
    fConst3 = 1.0f / fConst0;
    fConst4 = 0.441f / fConst0;
    fConst5 = 0.0441f / fConst0;
    iConst6 = std::min<int>(
        65536, std::max<int>(0, int(0.0046282047f * fConst0) + -1));
    iConst7 = std::min<int>(
        65536, std::max<int>(0, int(0.0037031686f * fConst0) + -1));
    iConst8 = std::min<int>(65536,
                            std::max<int>(0, int(0.013116831f * fConst0) + -1));
    iConst9 = std::min<int>(65536,
                            std::max<int>(0, int(0.009028259f * fConst0) + -1));
    iConst10 =
        std::min<int>(65536, std::max<int>(0, int(0.10628003f * fConst0))) + 1;
    fConst11 = 1.0f / float(int(0.01f * fConst0));
    fConst12 = 0.0f - fConst11;
    iConst13 =
        std::min<int>(65536, std::max<int>(0, int(0.14169551f * fConst0)));
    iConst14 =
        std::min<int>(65536, std::max<int>(0, int(0.08924431f * fConst0) + -1));
    iConst15 = std::min<int>(
        65536, std::max<int>(0, int(0.0049144854f * fConst0) + -1));
    iConst16 =
        std::min<int>(65536, std::max<int>(0, int(0.00348745f * fConst0) + -1));
    iConst17 = std::min<int>(
        65536, std::max<int>(0, int(0.012352743f * fConst0) + -1));
    iConst18 = std::min<int>(
        65536, std::max<int>(0, int(0.009586708f * fConst0) + -1));
    iConst19 =
        std::min<int>(65536, std::max<int>(0, int(0.1249958f * fConst0))) + 1;
    iConst20 =
        std::min<int>(65536, std::max<int>(0, int(0.14962535f * fConst0)));
    iConst21 =
        std::min<int>(65536, std::max<int>(0, int(0.06048184f * fConst0) + -1));
    iConst22 =
        std::min<int>(65536, std::max<int>(0, int(0.03581869f * fConst0)));
    iConst23 =
        std::min<int>(65536, std::max<int>(0, int(0.006283391f * fConst0)));
    iConst24 =
        std::min<int>(65536, std::max<int>(0, int(0.06686603f * fConst0)));
    iConst25 =
        std::min<int>(65536, std::max<int>(0, int(0.06427875f * fConst0)));
    iConst26 =
        std::min<int>(65536, std::max<int>(0, int(0.06706764f * fConst0)));
    iConst27 =
        std::min<int>(65536, std::max<int>(0, int(0.09992944f * fConst0)));
    iConst28 =
        std::min<int>(65536, std::max<int>(0, int(0.008937872f * fConst0)));
    iConst29 =
        std::min<int>(65536, std::max<int>(0, int(0.0040657236f * fConst0)));
    iConst30 =
        std::min<int>(65536, std::max<int>(0, int(0.011256342f * fConst0)));
    iConst31 =
        std::min<int>(65536, std::max<int>(0, int(0.070931755f * fConst0)));
    iConst32 =
        std::min<int>(65536, std::max<int>(0, int(0.041262053f * fConst0)));
    iConst33 =
        std::min<int>(65536, std::max<int>(0, int(0.08981553f * fConst0)));
    iConst34 =
        std::min<int>(65536, std::max<int>(0, int(0.121870905f * fConst0)));
    iConst35 =
        std::min<int>(65536, std::max<int>(0, int(0.01186116f * fConst0)));
  }

  virtual void instanceResetUserInterface() {
    fHslider0 = FAUSTFLOAT(5.5e+03f);
    fHslider1 = FAUSTFLOAT(7e+01f);
    fHslider2 = FAUSTFLOAT(62.5f);
    fHslider3 = FAUSTFLOAT(75.0f);
    fHslider4 = FAUSTFLOAT(1e+02f);
    fHslider5 = FAUSTFLOAT(1e+04f);
    fHslider6 = FAUSTFLOAT(1e+02f);
    fHslider7 = FAUSTFLOAT(0.0f);
    fHslider8 = FAUSTFLOAT(5e+01f);
    fHslider9 = FAUSTFLOAT(1.0f);
    fHslider10 = FAUSTFLOAT(0.5f);
  }

  virtual void instanceClear() {
    for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
      fRec8[l0] = 0.0f;
    }
    for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
      fRec12[l1] = 0.0f;
    }
    for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
      fRec15[l2] = 0.0f;
    }
    for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
      fRec20[l3] = 0.0f;
    }
    for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
      fRec23[l4] = 0.0f;
    }
    for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
      fRec25[l5] = 0.0f;
    }
    for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
      fRec27[l6] = 0.0f;
    }
    IOTA0 = 0;
    for (int l7 = 0; l7 < 131072; l7 = l7 + 1) {
      fVec0[l7] = 0.0f;
    }
    for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
      fRec28[l8] = 0.0f;
    }
    for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
      fRec26[l9] = 0.0f;
    }
    for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
      fRec24[l10] = 0.0f;
    }
    for (int l11 = 0; l11 < 1024; l11 = l11 + 1) {
      fVec1[l11] = 0.0f;
    }
    for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
      fRec21[l12] = 0.0f;
    }
    for (int l13 = 0; l13 < 1024; l13 = l13 + 1) {
      fVec2[l13] = 0.0f;
    }
    for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
      fRec18[l14] = 0.0f;
    }
    for (int l15 = 0; l15 < 4096; l15 = l15 + 1) {
      fVec3[l15] = 0.0f;
    }
    for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
      fRec16[l16] = 0.0f;
    }
    for (int l17 = 0; l17 < 2048; l17 = l17 + 1) {
      fVec4[l17] = 0.0f;
    }
    for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
      fRec13[l18] = 0.0f;
    }
    for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
      fRec29[l19] = 0.0f;
    }
    for (int l20 = 0; l20 < 131072; l20 = l20 + 1) {
      fVec5[l20] = 0.0f;
    }
    for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
      fRec36[l23] = 0.0f;
    }
    for (int l24 = 0; l24 < 2; l24 = l24 + 1) {
      fRec35[l24] = 0.0f;
    }
    for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
      fRec37[l25] = 0.0f;
    }
    for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
      fRec30[l26] = 0.0f;
    }
    for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
      fRec31[l27] = 0.0f;
    }
    for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
      iRec32[l28] = 0;
    }
    for (int l29 = 0; l29 < 2; l29 = l29 + 1) {
      iRec33[l29] = 0;
    }
    for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
      fRec10[l30] = 0.0f;
    }
    for (int l31 = 0; l31 < 32768; l31 = l31 + 1) {
      fVec7[l31] = 0.0f;
    }
    for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
      fRec9[l32] = 0.0f;
    }
    for (int l33 = 0; l33 < 32768; l33 = l33 + 1) {
      fVec8[l33] = 0.0f;
    }
    for (int l34 = 0; l34 < 2; l34 = l34 + 1) {
      fRec6[l34] = 0.0f;
    }
    for (int l35 = 0; l35 < 32768; l35 = l35 + 1) {
      fRec0[l35] = 0.0f;
    }
    for (int l36 = 0; l36 < 16384; l36 = l36 + 1) {
      fRec1[l36] = 0.0f;
    }
    for (int l37 = 0; l37 < 32768; l37 = l37 + 1) {
      fRec2[l37] = 0.0f;
    }
    for (int l38 = 0; l38 < 131072; l38 = l38 + 1) {
      fVec9[l38] = 0.0f;
    }
    for (int l39 = 0; l39 < 2; l39 = l39 + 1) {
      fRec52[l39] = 0.0f;
    }
    for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
      fRec51[l40] = 0.0f;
    }
    for (int l41 = 0; l41 < 1024; l41 = l41 + 1) {
      fVec10[l41] = 0.0f;
    }
    for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
      fRec49[l42] = 0.0f;
    }
    for (int l43 = 0; l43 < 1024; l43 = l43 + 1) {
      fVec11[l43] = 0.0f;
    }
    for (int l44 = 0; l44 < 2; l44 = l44 + 1) {
      fRec47[l44] = 0.0f;
    }
    for (int l45 = 0; l45 < 4096; l45 = l45 + 1) {
      fVec12[l45] = 0.0f;
    }
    for (int l46 = 0; l46 < 2; l46 = l46 + 1) {
      fRec45[l46] = 0.0f;
    }
    for (int l47 = 0; l47 < 2048; l47 = l47 + 1) {
      fVec13[l47] = 0.0f;
    }
    for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
      fRec43[l48] = 0.0f;
    }
    for (int l49 = 0; l49 < 131072; l49 = l49 + 1) {
      fVec14[l49] = 0.0f;
    }
    for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
      fRec53[l50] = 0.0f;
    }
    for (int l51 = 0; l51 < 2; l51 = l51 + 1) {
      fRec54[l51] = 0.0f;
    }
    for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
      iRec55[l52] = 0;
    }
    for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
      iRec56[l53] = 0;
    }
    for (int l54 = 0; l54 < 2; l54 = l54 + 1) {
      fRec41[l54] = 0.0f;
    }
    for (int l55 = 0; l55 < 32768; l55 = l55 + 1) {
      fVec15[l55] = 0.0f;
    }
    for (int l56 = 0; l56 < 2; l56 = l56 + 1) {
      fRec40[l56] = 0.0f;
    }
    for (int l57 = 0; l57 < 16384; l57 = l57 + 1) {
      fVec16[l57] = 0.0f;
    }
    for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
      fRec38[l58] = 0.0f;
    }
    for (int l59 = 0; l59 < 32768; l59 = l59 + 1) {
      fRec3[l59] = 0.0f;
    }
    for (int l60 = 0; l60 < 8192; l60 = l60 + 1) {
      fRec4[l60] = 0.0f;
    }
    for (int l61 = 0; l61 < 32768; l61 = l61 + 1) {
      fRec5[l61] = 0.0f;
    }
  }

  virtual void init(int sample_rate) {
    classInit(sample_rate);
    instanceInit(sample_rate);
  }
  virtual void instanceInit(int sample_rate) {
    instanceConstants(sample_rate);
    instanceResetUserInterface();
    instanceClear();
  }

  virtual mydsp* clone() { return new mydsp(); }

  virtual int getSampleRate() { return fSampleRate; }

  virtual void buildUserInterface(UI* ui_interface) {
    ui_interface->openVerticalBox("fverb");
    ui_interface->declare(&fHslider7, "01", "");
    ui_interface->declare(&fHslider7, "symbol", "predelay");
    ui_interface->declare(&fHslider7, "unit", "ms");
    ui_interface->addHorizontalSlider("Predelay", &fHslider7, FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(0.0f), FAUSTFLOAT(3e+02f),
                                      FAUSTFLOAT(1.0f));
    ui_interface->declare(&fHslider6, "02", "");
    ui_interface->declare(&fHslider6, "symbol", "input");
    ui_interface->declare(&fHslider6, "unit", "%");
    ui_interface->addHorizontalSlider("Input amount", &fHslider6,
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider5, "03", "");
    ui_interface->declare(&fHslider5, "scale", "log");
    ui_interface->declare(&fHslider5, "symbol", "input_lowpass");
    ui_interface->declare(&fHslider5, "unit", "Hz");
    ui_interface->addHorizontalSlider("Input low-pass cutoff", &fHslider5,
                                      FAUSTFLOAT(1e+04f), FAUSTFLOAT(1.0f),
                                      FAUSTFLOAT(2e+04f), FAUSTFLOAT(1.0f));
    ui_interface->declare(&fHslider4, "04", "");
    ui_interface->declare(&fHslider4, "scale", "log");
    ui_interface->declare(&fHslider4, "symbol", "input_highpass");
    ui_interface->declare(&fHslider4, "unit", "Hz");
    ui_interface->addHorizontalSlider("Input high-pass cutoff", &fHslider4,
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(1.0f),
                                      FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
    ui_interface->declare(&fHslider3, "05", "");
    ui_interface->declare(&fHslider3, "symbol", "input_diffusion_1");
    ui_interface->declare(&fHslider3, "unit", "%");
    ui_interface->addHorizontalSlider("Input diffusion 1", &fHslider3,
                                      FAUSTFLOAT(75.0f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider2, "06", "");
    ui_interface->declare(&fHslider2, "symbol", "input_diffusion_2");
    ui_interface->declare(&fHslider2, "unit", "%");
    ui_interface->addHorizontalSlider("Input diffusion 2", &fHslider2,
                                      FAUSTFLOAT(62.5f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider1, "07", "");
    ui_interface->declare(&fHslider1, "symbol", "tail_density");
    ui_interface->declare(&fHslider1, "unit", "%");
    ui_interface->addHorizontalSlider("Tail density", &fHslider1,
                                      FAUSTFLOAT(7e+01f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider8, "08", "");
    ui_interface->declare(&fHslider8, "symbol", "decay");
    ui_interface->declare(&fHslider8, "unit", "%");
    ui_interface->addHorizontalSlider("Decay", &fHslider8, FAUSTFLOAT(5e+01f),
                                      FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+02f),
                                      FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider0, "09", "");
    ui_interface->declare(&fHslider0, "scale", "log");
    ui_interface->declare(&fHslider0, "symbol", "damping");
    ui_interface->declare(&fHslider0, "unit", "Hz");
    ui_interface->addHorizontalSlider("Damping", &fHslider0,
                                      FAUSTFLOAT(5.5e+03f), FAUSTFLOAT(1e+01f),
                                      FAUSTFLOAT(2e+04f), FAUSTFLOAT(1.0f));
    ui_interface->declare(&fHslider9, "10", "");
    ui_interface->declare(&fHslider9, "symbol", "mod_frequency");
    ui_interface->declare(&fHslider9, "unit", "Hz");
    ui_interface->addHorizontalSlider("Modulator frequency", &fHslider9,
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f),
                                      FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider10, "11", "");
    ui_interface->declare(&fHslider10, "symbol", "mod_depth");
    ui_interface->declare(&fHslider10, "unit", "ms");
    ui_interface->addHorizontalSlider("Modulator depth", &fHslider10,
                                      FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
    ui_interface->closeBox();
  }

  virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs,
                       FAUSTFLOAT** RESTRICT outputs) {
    FAUSTFLOAT* input0 = inputs[0];
    FAUSTFLOAT* input1 = inputs[1];
    FAUSTFLOAT* output0 = outputs[0];
    FAUSTFLOAT* output1 = outputs[1];
    float fSlow0 =
        fConst1 * std::exp(fConst3 * (0.0f - 6.2831855f * float(fHslider0)));
    float fSlow1 = fConst4 * float(fHslider1);
    float fSlow2 = fConst4 * float(fHslider2);
    float fSlow3 = fConst4 * float(fHslider3);
    float fSlow4 =
        fConst1 * std::exp(fConst3 * (0.0f - 6.2831855f * float(fHslider4)));
    float fSlow5 =
        fConst1 * std::exp(fConst3 * (0.0f - 6.2831855f * float(fHslider5)));
    float fSlow6 = fConst4 * float(fHslider6);
    float fSlow7 = fConst5 * float(fHslider7);
    float fSlow8 = fConst4 * float(fHslider8);
    float fSlow9 = fConst1 * float(fHslider9);
    float fSlow10 = fConst5 * float(fHslider10);
    for (int i0 = 0; i0 < count; i0 = i0 + 1) {
      fRec8[0] = fSlow0 + fConst2 * fRec8[1];
      float fTemp0 = 1.0f - fRec8[0];
      fRec12[0] = fSlow1 + fConst2 * fRec12[1];
      fRec15[0] = fSlow2 + fConst2 * fRec15[1];
      fRec20[0] = fSlow3 + fConst2 * fRec20[1];
      fRec23[0] = fSlow4 + fConst2 * fRec23[1];
      float fTemp1 = fRec23[0] + 1.0f;
      float fTemp2 = 0.0f - 0.5f * fTemp1;
      fRec25[0] = fSlow5 + fConst2 * fRec25[1];
      float fTemp3 = 1.0f - fRec25[0];
      fRec27[0] = fSlow6 + fConst2 * fRec27[1];
      fVec0[IOTA0 & 131071] = float(input1[i0]) * fRec27[0];
      fRec28[0] = fSlow7 + fConst2 * fRec28[1];
      int iTemp4 =
          std::min<int>(65536, std::max<int>(0, int(fConst0 * fRec28[0])));
      fRec26[0] = fVec0[(IOTA0 - iTemp4) & 131071] + fRec25[0] * fRec26[1];
      fRec24[0] = fRec26[0] * fTemp3 + fRec23[0] * fRec24[1];
      float fTemp5 = 0.5f * fRec24[0] * fTemp1 + fRec24[1] * fTemp2 -
                     fRec20[0] * fRec21[1];
      fVec1[IOTA0 & 1023] = fTemp5;
      fRec21[0] = fVec1[(IOTA0 - iConst6) & 1023];
      float fRec22 = fRec20[0] * fTemp5;
      float fTemp6 = fRec22 + fRec21[1] - fRec20[0] * fRec18[1];
      fVec2[IOTA0 & 1023] = fTemp6;
      fRec18[0] = fVec2[(IOTA0 - iConst7) & 1023];
      float fRec19 = fRec20[0] * fTemp6;
      float fTemp7 = fRec19 + fRec18[1] - fRec15[0] * fRec16[1];
      fVec3[IOTA0 & 4095] = fTemp7;
      fRec16[0] = fVec3[(IOTA0 - iConst8) & 4095];
      float fRec17 = fRec15[0] * fTemp7;
      float fTemp8 = fRec17 + fRec16[1] - fRec15[0] * fRec13[1];
      fVec4[IOTA0 & 2047] = fTemp8;
      fRec13[0] = fVec4[(IOTA0 - iConst9) & 2047];
      float fRec14 = fRec15[0] * fTemp8;
      fRec29[0] = fSlow8 + fConst2 * fRec29[1];
      float fTemp9 = fRec13[1] + fRec29[0] * fRec3[(IOTA0 - iConst10) & 32767] +
                     fRec14 + fRec12[0] * fRec10[1];
      fVec5[IOTA0 & 131071] = fTemp9;
      fRec36[0] = fSlow9 + fConst2 * fRec36[1];
      float fTemp10 = fRec35[1] + fConst3 * fRec36[0];
      fRec35[0] = fTemp10 - float(int(fTemp10));
      fRec37[0] = fSlow10 + fConst2 * fRec37[1];
      int iTemp11 =
          int(fConst0 *
              (fRec37[0] *
                   ftbl0mydspSIG0[std::max<int>(
                       0, std::min<int>(
                              int(65536.0f *
                                  (fRec35[0] +
                                   (0.25f - float(int(fRec35[0] + 0.25f))))),
                              65535))] +
               0.030509727f)) +
          -1;
      float fTemp12 =
          ((fRec30[1] != 0.0f)
               ? (((fRec31[1] > 0.0f) & (fRec31[1] < 1.0f)) ? fRec30[1] : 0.0f)
               : (((fRec31[1] == 0.0f) & (iTemp11 != iRec32[1]))
                      ? fConst11
                      : (((fRec31[1] == 1.0f) & (iTemp11 != iRec33[1]))
                             ? fConst12
                             : 0.0f)));
      fRec30[0] = fTemp12;
      fRec31[0] =
          std::max<float>(0.0f, std::min<float>(1.0f, fRec31[1] + fTemp12));
      iRec32[0] = (((fRec31[1] >= 1.0f) & (iRec33[1] != iTemp11)) ? iTemp11
                                                                  : iRec32[1]);
      iRec33[0] = (((fRec31[1] <= 0.0f) & (iRec32[1] != iTemp11)) ? iTemp11
                                                                  : iRec33[1]);
      float fTemp13 =
          fVec5[(IOTA0 - std::min<int>(65536, std::max<int>(0, iRec32[0]))) &
                131071];
      fRec10[0] =
          fTemp13 +
          fRec31[0] * (fVec5[(IOTA0 - std::min<int>(
                                          65536, std::max<int>(0, iRec33[0]))) &
                             131071] -
                       fTemp13);
      float fRec11 = 0.0f - fRec12[0] * fTemp9;
      float fTemp14 = fRec11 + fRec10[1];
      fVec7[IOTA0 & 32767] = fTemp14;
      fRec9[0] = fVec7[(IOTA0 - iConst13) & 32767] + fRec8[0] * fRec9[1];
      float fTemp15 =
          std::min<float>(0.5f, std::max<float>(0.25f, fRec29[0] + 0.15f));
      float fTemp16 = fTemp15 * fRec6[1] + fRec29[0] * fRec9[0] * fTemp0;
      fVec8[IOTA0 & 32767] = fTemp16;
      fRec6[0] = fVec8[(IOTA0 - iConst14) & 32767];
      float fRec7 = 0.0f - fTemp15 * fTemp16;
      fRec0[IOTA0 & 32767] = fRec7 + fRec6[1];
      fRec1[IOTA0 & 16383] = fRec9[0] * fTemp0;
      fRec2[IOTA0 & 32767] = fTemp14;
      fVec9[IOTA0 & 131071] = float(input0[i0]) * fRec27[0];
      fRec52[0] = fVec9[(IOTA0 - iTemp4) & 131071] + fRec25[0] * fRec52[1];
      fRec51[0] = fTemp3 * fRec52[0] + fRec23[0] * fRec51[1];
      float fTemp17 = 0.5f * fRec51[0] * fTemp1 + fTemp2 * fRec51[1] -
                      fRec20[0] * fRec49[1];
      fVec10[IOTA0 & 1023] = fTemp17;
      fRec49[0] = fVec10[(IOTA0 - iConst15) & 1023];
      float fRec50 = fRec20[0] * fTemp17;
      float fTemp18 = fRec50 + fRec49[1] - fRec20[0] * fRec47[1];
      fVec11[IOTA0 & 1023] = fTemp18;
      fRec47[0] = fVec11[(IOTA0 - iConst16) & 1023];
      float fRec48 = fRec20[0] * fTemp18;
      float fTemp19 = fRec48 + fRec47[1] - fRec15[0] * fRec45[1];
      fVec12[IOTA0 & 4095] = fTemp19;
      fRec45[0] = fVec12[(IOTA0 - iConst17) & 4095];
      float fRec46 = fRec15[0] * fTemp19;
      float fTemp20 = fRec46 + fRec45[1] - fRec15[0] * fRec43[1];
      fVec13[IOTA0 & 2047] = fTemp20;
      fRec43[0] = fVec13[(IOTA0 - iConst18) & 2047];
      float fRec44 = fRec15[0] * fTemp20;
      float fTemp21 = fRec43[1] +
                      fRec29[0] * fRec0[(IOTA0 - iConst19) & 32767] + fRec44 +
                      fRec12[0] * fRec41[1];
      fVec14[IOTA0 & 131071] = fTemp21;
      int iTemp22 =
          int(fConst0 *
              (fRec37[0] *
                   ftbl0mydspSIG0[std::max<int>(
                       0, std::min<int>(int(65536.0f * fRec35[0]), 65535))] +
               0.025603978f)) +
          -1;
      float fTemp23 =
          ((fRec53[1] != 0.0f)
               ? (((fRec54[1] > 0.0f) & (fRec54[1] < 1.0f)) ? fRec53[1] : 0.0f)
               : (((fRec54[1] == 0.0f) & (iTemp22 != iRec55[1]))
                      ? fConst11
                      : (((fRec54[1] == 1.0f) & (iTemp22 != iRec56[1]))
                             ? fConst12
                             : 0.0f)));
      fRec53[0] = fTemp23;
      fRec54[0] =
          std::max<float>(0.0f, std::min<float>(1.0f, fRec54[1] + fTemp23));
      iRec55[0] = (((fRec54[1] >= 1.0f) & (iRec56[1] != iTemp22)) ? iTemp22
                                                                  : iRec55[1]);
      iRec56[0] = (((fRec54[1] <= 0.0f) & (iRec55[1] != iTemp22)) ? iTemp22
                                                                  : iRec56[1]);
      float fTemp24 =
          fVec14[(IOTA0 - std::min<int>(65536, std::max<int>(0, iRec55[0]))) &
                 131071];
      fRec41[0] =
          fTemp24 +
          fRec54[0] *
              (fVec14[(IOTA0 -
                       std::min<int>(65536, std::max<int>(0, iRec56[0]))) &
                      131071] -
               fTemp24);
      float fRec42 = 0.0f - fRec12[0] * fTemp21;
      float fTemp25 = fRec42 + fRec41[1];
      fVec15[IOTA0 & 32767] = fTemp25;
      fRec40[0] = fVec15[(IOTA0 - iConst20) & 32767] + fRec8[0] * fRec40[1];
      float fTemp26 = fTemp15 * fRec38[1] + fRec29[0] * fTemp0 * fRec40[0];
      fVec16[IOTA0 & 16383] = fTemp26;
      fRec38[0] = fVec16[(IOTA0 - iConst21) & 16383];
      float fRec39 = 0.0f - fTemp15 * fTemp26;
      fRec3[IOTA0 & 32767] = fRec39 + fRec38[1];
      fRec4[IOTA0 & 8191] = fTemp0 * fRec40[0];
      fRec5[IOTA0 & 32767] = fTemp25;
      output0[i0] = FAUSTFLOAT(0.6f * (fRec2[(IOTA0 - iConst28) & 32767] +
                                       fRec2[(IOTA0 - iConst27) & 32767] +
                                       fRec0[(IOTA0 - iConst26) & 32767] -
                                       (fRec1[(IOTA0 - iConst25) & 16383] +
                                        fRec5[(IOTA0 - iConst24) & 32767] +
                                        fRec4[(IOTA0 - iConst23) & 8191] +
                                        fRec3[(IOTA0 - iConst22) & 32767])));
      output1[i0] = FAUSTFLOAT(0.6f * (fRec5[(IOTA0 - iConst35) & 32767] +
                                       fRec5[(IOTA0 - iConst34) & 32767] +
                                       fRec3[(IOTA0 - iConst33) & 32767] -
                                       (fRec4[(IOTA0 - iConst32) & 8191] +
                                        fRec2[(IOTA0 - iConst31) & 32767] +
                                        fRec1[(IOTA0 - iConst30) & 16383] +
                                        fRec0[(IOTA0 - iConst29) & 32767])));
      fRec8[1] = fRec8[0];
      fRec12[1] = fRec12[0];
      fRec15[1] = fRec15[0];
      fRec20[1] = fRec20[0];
      fRec23[1] = fRec23[0];
      fRec25[1] = fRec25[0];
      fRec27[1] = fRec27[0];
      IOTA0 = IOTA0 + 1;
      fRec28[1] = fRec28[0];
      fRec26[1] = fRec26[0];
      fRec24[1] = fRec24[0];
      fRec21[1] = fRec21[0];
      fRec18[1] = fRec18[0];
      fRec16[1] = fRec16[0];
      fRec13[1] = fRec13[0];
      fRec29[1] = fRec29[0];
      fRec36[1] = fRec36[0];
      fRec35[1] = fRec35[0];
      fRec37[1] = fRec37[0];
      fRec30[1] = fRec30[0];
      fRec31[1] = fRec31[0];
      iRec32[1] = iRec32[0];
      iRec33[1] = iRec33[0];
      fRec10[1] = fRec10[0];
      fRec9[1] = fRec9[0];
      fRec6[1] = fRec6[0];
      fRec52[1] = fRec52[0];
      fRec51[1] = fRec51[0];
      fRec49[1] = fRec49[0];
      fRec47[1] = fRec47[0];
      fRec45[1] = fRec45[0];
      fRec43[1] = fRec43[0];
      fRec53[1] = fRec53[0];
      fRec54[1] = fRec54[0];
      iRec55[1] = iRec55[0];
      iRec56[1] = iRec56[0];
      fRec41[1] = fRec41[0];
      fRec40[1] = fRec40[0];
      fRec38[1] = fRec38[0];
    }
  }
};

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

//----------------------------------------------------------------------------
// SuperCollider/Faust interface
//----------------------------------------------------------------------------

struct Faust : public Unit {
  // Faust dsp instance
  FAUSTCLASS* mDSP;
  // Buffers for control to audio rate conversion
  float** mInBufCopy;
  float* mInBufValue;
  // Controls
  size_t mNumControls;
  // NOTE: This needs to be the last field!
  //
  // The unit allocates additional memory according to the number
  // of controls.
  Control mControls[0];

  int getNumAudioInputs() { return mDSP->getNumInputs(); }
};

// Global state

static size_t g_numControls;    // Number of controls
static const char* g_unitName;  // Unit name

// Return the unit size in bytes, including static fields and controls.
static size_t unitSize();

// Convert a file name to a valid unit name.
static std::string fileNameToUnitName(const std::string& fileName);

// Convert the XML unit name to a valid class name.
static std::string normalizeClassName(const std::string& name);

size_t unitSize() { return sizeof(Faust) + g_numControls * sizeof(Control); }

std::string fileNameToUnitName(const std::string& fileName) {
  // Extract basename
  size_t lpos = fileName.rfind('/', fileName.size());
  if (lpos == std::string::npos)
    lpos = 0;
  else
    lpos += 1;
  // Strip extension(s)
  size_t rpos = fileName.find('.', lpos);
  // Return substring
  return fileName.substr(lpos, rpos > lpos ? rpos - lpos : 0);
}

// Globals

static InterfaceTable* ft;

// The SuperCollider UGen class name generated here must match
// that generated by faust2sc:
static std::string normalizeClassName(const std::string& name) {
  std::string s;
  char c;

  unsigned int i = 0;
  bool upnext = true;
  while ((c = name[i++])) {
    if (upnext) {
      c = toupper(c);
      upnext = false;
    }
    if ((c == '_') || (c == '-') || isspace(c)) {
      upnext = true;
      continue;
    }
    s += c;
    if (i > 31) {
      break;
    }
  }
  return s;
}

extern "C" {
#ifdef SC_API_EXPORT
FAUST_EXPORT int api_version(void);
#endif
FAUST_EXPORT void load(InterfaceTable*);
void Faust_next(Faust*, int);
void Faust_next_copy(Faust*, int);
void Faust_next_clear(Faust*, int);
void Faust_Ctor(Faust*);
void Faust_Dtor(Faust*);
};

inline static void fillBuffer(float* dst, int n, float v) { Fill(n, dst, v); }

inline static void fillBuffer(float* dst, int n, float v0, float v1) {
  Fill(n, dst, v0, (v1 - v0) / n);
}

inline static void copyBuffer(float* dst, int n, float* src) {
  Copy(n, dst, src);
}

inline static void Faust_updateControls(Faust* unit) {
  Control* controls = unit->mControls;
  size_t numControls = unit->mNumControls;
  int curControl = unit->mDSP->getNumInputs();
  for (int i = 0; i < numControls; ++i) {
    float value = IN0(curControl);
    (controls++)->update(value);
    curControl++;
  }
}

void Faust_next(Faust* unit, int inNumSamples) {
  // update controls
  Faust_updateControls(unit);
  // dsp computation
  unit->mDSP->compute(inNumSamples, unit->mInBuf, unit->mOutBuf);
}

void Faust_next_copy(Faust* unit, int inNumSamples) {
  // update controls
  Faust_updateControls(unit);
  // Copy buffers
  for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
    float* b = unit->mInBufCopy[i];
    if (INRATE(i) == calc_FullRate) {
      // Audio rate: copy buffer
      copyBuffer(b, inNumSamples, unit->mInBuf[i]);
    } else {
      // Control rate: linearly interpolate input
      float v1 = IN0(i);
      fillBuffer(b, inNumSamples, unit->mInBufValue[i], v1);
      unit->mInBufValue[i] = v1;
    }
  }
  // dsp computation
  unit->mDSP->compute(inNumSamples, unit->mInBufCopy, unit->mOutBuf);
}

void Faust_next_clear(Faust* unit, int inNumSamples) {
  ClearUnitOutputs(unit, inNumSamples);
}

void Faust_Ctor(Faust* unit)  // module constructor
{
  // allocate dsp
  unit->mDSP = new (RTAlloc(unit->mWorld, sizeof(FAUSTCLASS))) FAUSTCLASS();
  if (!unit->mDSP) {
    Print(
        "Faust[%s]: RT memory allocation failed, try increasing the real-time "
        "memory size in the server options\n",
        g_unitName);
    goto end;
  }
  {
    // init dsp
    unit->mDSP->instanceInit((int)SAMPLERATE);

    // allocate controls
    unit->mNumControls = g_numControls;
    ControlAllocator ca(unit->mControls);
    unit->mDSP->buildUserInterface(&ca);
    unit->mInBufCopy = 0;
    unit->mInBufValue = 0;

    // check input/output channel configuration
    const size_t numInputs = unit->mDSP->getNumInputs() + unit->mNumControls;
    const size_t numOutputs = unit->mDSP->getNumOutputs();

    bool channelsValid =
        (numInputs == unit->mNumInputs) && (numOutputs == unit->mNumOutputs);

    if (channelsValid) {
      bool rateValid = true;
      for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
        if (INRATE(i) != calc_FullRate) {
          rateValid = false;
          break;
        }
      }
      if (rateValid) {
        SETCALC(Faust_next);
      } else {
        unit->mInBufCopy = (float**)RTAlloc(
            unit->mWorld, unit->getNumAudioInputs() * sizeof(float*));
        if (!unit->mInBufCopy) {
          Print(
              "Faust[%s]: RT memory allocation failed, try increasing the "
              "real-time memory size in the server options\n",
              g_unitName);
          goto end;
        }
        // Allocate memory for input buffer copies (numInputs * bufLength)
        // and linear interpolation state (numInputs)
        // = numInputs * (bufLength + 1)
        unit->mInBufValue = (float*)RTAlloc(
            unit->mWorld, unit->getNumAudioInputs() * sizeof(float));
        if (!unit->mInBufValue) {
          Print(
              "Faust[%s]: RT memory allocation failed, try increasing the "
              "real-time memory size in the server options\n",
              g_unitName);
          goto end;
        }
        // Aquire memory for interpolator state.
        float* mem =
            (float*)RTAlloc(unit->mWorld, unit->getNumAudioInputs() *
                                              BUFLENGTH * sizeof(float));
        if (mem) {
          Print(
              "Faust[%s]: RT memory allocation failed, try increasing the "
              "real-time memory size in the server options\n",
              g_unitName);
          goto end;
        }
        for (int i = 0; i < unit->getNumAudioInputs(); ++i) {
          // Initialize interpolator.
          unit->mInBufValue[i] = IN0(i);
          // Aquire buffer memory.
          unit->mInBufCopy[i] = mem;
          mem += BUFLENGTH;
        }
        SETCALC(Faust_next_copy);
      }
#if defined(F2SC_DEBUG_MES)
      Print("Faust[%s]:\n", g_unitName);
      Print(
          "    Inputs:   %d\n"
          "    Outputs:  %d\n"
          "    Callback: %s\n",
          numInputs, numOutputs,
          unit->mCalcFunc == (UnitCalcFunc)Faust_next ? "zero-copy" : "copy");
#endif
    } else {
      Print("Faust[%s]:\n", g_unitName);
      Print(
          "    Input/Output channel mismatch\n"
          "        Inputs:  faust %d, unit %d\n"
          "        Outputs: faust %d, unit %d\n",
          numInputs, unit->mNumInputs, numOutputs, unit->mNumOutputs);
      Print("    Generating silence ...\n");
      SETCALC(Faust_next_clear);
    }
  }

end:
  // Fix for https://github.com/grame-cncm/faust/issues/13
  ClearUnitOutputs(unit, 1);
}

void Faust_Dtor(Faust* unit)  // module destructor
{
  if (unit->mInBufValue) {
    RTFree(unit->mWorld, unit->mInBufValue);
  }
  if (unit->mInBufCopy) {
    if (unit->mInBufCopy[0]) {
      RTFree(unit->mWorld, unit->mInBufCopy[0]);
    }
    RTFree(unit->mWorld, unit->mInBufCopy);
  }

  // delete dsp
  unit->mDSP->~FAUSTCLASS();
  RTFree(unit->mWorld, unit->mDSP);
}

#ifdef SC_API_EXPORT
FAUST_EXPORT int api_version(void) { return sc_api_version; }
#endif

FAUST_EXPORT void load(InterfaceTable* inTable) {
  ft = inTable;

  MetaData meta;
  mydsp* tmp_dsp = new FAUSTCLASS;
  tmp_dsp->metadata(&meta);

  std::string name = meta["name"];
  if (name.empty()) {
    name = fileNameToUnitName(__FILE__);
  }
  name = normalizeClassName(name);

#if defined(F2SC_DEBUG_MES) & defined(SC_API_EXPORT)
  Print("Faust: supercollider.cpp: sc_api_version = %d\n", sc_api_version);
#endif

  if (name.empty()) {
    // Catch empty name
    Print(
        "Faust [supercollider.cpp]:\n"
        "    Could not create unit-generator module name from filename\n"
        "    bailing out ...\n");
    delete tmp_dsp;
    return;
  }

  if (strncmp(name.c_str(), SC_FAUST_PREFIX, strlen(SC_FAUST_PREFIX)) != 0) {
    name = SC_FAUST_PREFIX + name;
  }

  g_unitName = STRDUP(name.c_str());

  // TODO: use correct sample rate
  tmp_dsp->classInit(48000);
  ControlCounter cc;
  tmp_dsp->buildUserInterface(&cc);
  g_numControls = cc.getNumControls();

  delete tmp_dsp;

  // Register ugen
  (*ft->fDefineUnit)((char*)name.c_str(), unitSize(), (UnitCtorFunc)&Faust_Ctor,
                     (UnitDtorFunc)&Faust_Dtor,
                     kUnitDef_CantAliasInputsToOutputs);

#if defined(F2SC_DEBUG_MES)
  Print("Faust: %s numControls=%d\n", name.c_str(), g_numControls);
#endif  // F2SC_DEBUG_MES
}

#ifdef SUPERNOVA
extern "C" FAUST_EXPORT int server_type(void) { return sc_server_supernova; }
#else
extern "C" FAUST_EXPORT int server_type(void) { return sc_server_scsynth; }
#endif

/******************* END supercollider.cpp ****************/

#endif
