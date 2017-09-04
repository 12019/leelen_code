#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <QDialog>
#include <QWSPointerCalibrationData>

class CalibrationDialog : public QDialog
{
	public:
		CalibrationDialog(QWidget *parent = 0);
		~CalibrationDialog();
		int exec();
		void close();

	protected:
		void paintEvent(QPaintEvent*);
		void mouseReleaseEvent(QMouseEvent*);
		void accept();

	private:
		QWSPointerCalibrationData data;
		int pressCount;
};

#endif // CALIBRATION_H
