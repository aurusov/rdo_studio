#ifndef _RDO_STUDIO_FRAME_MANAGER_H_
#define _RDO_STUDIO_FRAME_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <vector>
#include <map>
#include <memory>
#include <boost/function.hpp>
#include <QMdiSubWindow>
#include <QTreeWidget>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_view.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace animation {

struct Frame;

}} // namespace rdo::animation

namespace rdo { namespace gui { namespace frame {

class Manager: public QObject
{
Q_OBJECT

public:
	typedef boost::function<void (std::size_t)> OnChangeFrame;

	Manager(const OnChangeFrame& onChangeFrame);
	virtual ~Manager();

	void insertFrame (const QString& frameName );
	void insertBitmap(const QString& bitmapName);

	std::size_t findFrameIndex(const QTreeWidgetItem* pTreeWidgetItem) const;
	std::size_t findFrameIndex(const View* pView) const;
	std::size_t findFrameIndex(const Content* pContent) const;

	bool isShowing() const;
	const QString& getFrameName(std::size_t index) const;
	View* getFrameView(std::size_t index) const;
	View* getFrameViewFirst() const;
	std::size_t count() const;
	bool isChanged();

	void areaDown(std::size_t frameIndex, const QPoint& point) const;

	View* createView(std::size_t index);
	void disconnectView(const View* pView);
	void closeAll();
	void clear();

	std::size_t getLastShowedFrame() const;
	void setLastShowedFrame(std::size_t index);
	void setCurrentShowingFrame(std::size_t index);
	void resetCurrentShowingFrame(std::size_t index);
	void showFrame(const rdo::animation::Frame* const pFrame, std::size_t index);
	void showNextFrame();
	void showPrevFrame();
	void showFrame(std::size_t index);
	bool canShowNextFrame() const;
	bool canShowPrevFrame() const;
	void updateStyles() const;

private:
	struct Frame
	{
		 Frame();
		~Frame();

		QTreeWidgetItem* m_pTreeWidgetItem;
		QString m_name;
		View* m_pView;
		Content* m_pContent;
		rdo::gui::animation::AreaList m_areaList;

	private:
		void clear();
	};

	typedef std::vector<Frame*> FrameList;

	FrameList m_frameList;
	rdo::gui::BitmapList m_bitmapList;
	std::size_t m_lastShowedFrame;
	std::size_t m_currentShowingFrame;
	bool m_changed;
	OnChangeFrame m_onChangeFrame;

private slots:
	void onSubWindowActivated         (QMdiSubWindow* pWindow);
	void onTreeWidgetItemDoubleClicked(QTreeWidgetItem* pTreeWidgetItem, int);
};

}}} // namespace rdo::gui::frame

#endif // _RDO_STUDIO_FRAME_MANAGER_H_
