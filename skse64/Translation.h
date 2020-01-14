#pragma once

#include <shlobj.h>
#include <string>
#include "common/IFileStream.h"
#include "skse64/GameStreams.h"
#include "skse64/GameSettings.h"
#include "skse64/ScaleformState.h"


class BSScaleformTranslator;

namespace Translation
{
	void ImportTranslationFiles(BSScaleformTranslator * translator);
}
